#pragma once

#include <memory>
#include <optional>
#include <set>
#include <tuple>

#include "obelisk/ConcurrentQueueT.hpp"
#include "obelisk/MutexedObject.hpp"
#include "obelisk/SimpleAsyncTask.h"

namespace obelisk
{
// useSharedHandling==true uses a single queue and consumer thread for each combination of Args
// useSharedHandling==false uses a queue/thread per event
template <bool useSharedHandling, typename... Args> class AsyncEventHandler
{
	using OptArgPack = std::optional<std::tuple<Args...>>;

	// EventParameterQueue shared between all instances of AsyncEventHandler
	using SharedEventParameterQueueValueT = std::pair<AsyncEventHandler *, OptArgPack>;
	using SharedEventParameterQueue = MultipleProducerSingleConsumerQueue<SharedEventParameterQueueValueT>;

	class SharedEventHandlerImpl
	{
		MutexedObject<std::set<AsyncEventHandler *>> activeObjects;
		SimpleAsyncTask sharedConsumer;
		obelisk::Mutex sharedHandlerMutex;
		unsigned objectCount;

	  public:
		SharedEventParameterQueue sharedQueue;

		void consumerFunc()
		{
			// Keep getting arguments until nullptr is passed for handler
			bool run = true;
			while (run)
			{
				auto args = sharedQueue.waitForPop();
				if (args.second)
				{
					activeObjects.lockedAccess([&args](const std::set<AsyncEventHandler *> &objects) {
						if (auto fnIt = objects.find(args.first); fnIt != objects.end())
						{
							(*fnIt)->invokeHandlers(*args.second);
						}
					});
				}
				else
					run = false;
			}
		}

		void onConstruct(AsyncEventHandler &handler)
		{
			lockAndCall(sharedHandlerMutex, [this]() {
				if (objectCount == 0)
				{
					// Create consumer
					sharedConsumer = SimpleAsyncTask([this]() { consumerFunc(); });
				}
				++objectCount;
			});

			activeObjects.lockedModify([&handler](std::set<AsyncEventHandler *> &objects) { objects.insert(&handler); });
		}

		void onDestruct(AsyncEventHandler &handler)
		{
			activeObjects.lockedModify([&handler](std::set<AsyncEventHandler *> &objects) { objects.erase(&handler); });

			LockGuard lk(sharedHandlerMutex);
			assert(objectCount > 0);
			--objectCount;
			if (objectCount == 0)
			{
				// Destroy consumer
				sharedQueue.clear();
				sharedQueue.push({nullptr, {}});
				sharedConsumer.wait();
			}
		}
	};

	static SharedEventHandlerImpl &getSharedHandler()
	{
		static_assert(useSharedHandling);
		static SharedEventHandlerImpl sharedHandler;
		return sharedHandler;
	}

	template <bool shared> class ImplT;

	template <> class ImplT<true>
	{
	  public:
		ImplT(AsyncEventHandler<true, Args...> &parent) : parent(parent) { getSharedHandler().onConstruct(parent); }
		~ImplT() { getSharedHandler().onDestruct(parent); }
		bool empty() const
		{
			const std::function<bool(const std::deque<SharedEventParameterQueueValueT> &q)> fn = [this](const std::deque<SharedEventParameterQueueValueT> &q) {
				for (auto const &i : q)
					if (i.first == &this->parent)
						return false;

				return true;
			};

			return getSharedHandler().sharedQueue.safeAccessQueue(fn);
		}
		void deferredEventInvoke(OptArgPack &&argPack) { getSharedHandler().sharedQueue.push({&parent, argPack}); }

	  private:
		AsyncEventHandler<true, Args...> &parent;
	};

	template <> class ImplT<false>
	{
	  public:
		ImplT(AsyncEventHandler<false, Args...> &parent) : parent(parent)
		{
			consumer = SimpleAsyncTask([this]() {
				// Keep getting arguments until nullptr is passed
				while (auto args = queue.waitForPop())
				{
					this->parent.invokeHandlers(*args);
				}
			});
		}
		~ImplT()
		{
			queue.clear();
			queue.push({});
			consumer.wait();
		}
		bool empty() const { return queue.empty(); }
		void deferredEventInvoke(OptArgPack &&argPack) { queue.push(argPack); }

	  private:
		AsyncEventHandler<false, Args...> &parent;
		SimpleAsyncTask consumer;
		MultipleProducerSingleConsumerQueue<OptArgPack> queue;
	};

	// friend ImplT<useSharedHandling>;

	// Sequence generator helper structures
	// These are used to unpack a tuple into arguments for a function call
	template <std::size_t... Ts> struct Index
	{
	};

	template <std::size_t N, std::size_t... Ts> struct GenerateSequence : GenerateSequence<N - 1, N - 1, Ts...>
	{
	};

	template <std::size_t... Ts> struct GenerateSequence<0, Ts...> : Index<Ts...>
	{
	};

  public:
	template <typename InvokeEventHandlersFunc> AsyncEventHandler(InvokeEventHandlersFunc invokeFunc) : invokeFunc(invokeFunc), impl(*this) {}

	~AsyncEventHandler() = default;

	bool empty() const { return impl.empty(); }

	void deferredEventInvoke(Args... args) { impl.deferredEventInvoke(std::make_tuple(std::forward<Args>(args)...)); }

  private:
	template <std::size_t... Is> void invokeHandlers(std::tuple<Args...> &tup, Index<Is...>) { invokeFunc(std::get<Is>(tup)...); }

	void invokeHandlers(std::tuple<Args...> &tup) { invokeHandlers(tup, GenerateSequence<sizeof...(Args)>{}); }

	std::function<void(Args...)> invokeFunc;
	ImplT<useSharedHandling> impl;
};
} // namespace obelisk