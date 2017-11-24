#pragma once

#include "EventObserver.h"
#include "AsyncEvent.h"

#include <functional>
#include <atomic>
#include <cassert>

namespace ObeliskTest
{
	template <typename...Args>
	class ObserverWithSingleCallback : public obelisk::EventObserver
	{
	public:

		ObserverWithSingleCallback() = delete;
		ObserverWithSingleCallback(ObserverWithSingleCallback &) = delete;

		template <typename Func>
		ObserverWithSingleCallback(Func&& fn) :
			fn(std::forward<Func>(fn)),
			callCount(0)
		{
			assert(this->fn != nullptr);
		}

		ObserverWithSingleCallback(ObserverWithSingleCallback&& o) :
			fn(std::move(o.fn)),
			callCount(o.callCount.load())
		{
			assert(this->fn != nullptr);
		}

		~ObserverWithSingleCallback()
		{
			unregisterFromAllEvents();
		}

		void callFunc(Args...args)
		{
			fn(args...);

			++callCount;
		}

		size_t getCallCount() const
		{
			return callCount.load();
		}

	private:
		std::function<void(Args...)> fn;
		std::atomic<size_t> callCount;
	};

	template <size_t eventDelay, bool allowDuplicates, typename...Args>
	class SingleAsyncEventObject : public obelisk::SubjectWithEvents
	{
	public:

		SingleAsyncEventObject(Args...args) :
			testEvent(*this, L"Invoke Stuff"),
			invokeCallCount(0)
		{
			invokeFunc = std::bind(&SingleAsyncEventObject::invokeHandlers, this, args...);
		}

		~SingleAsyncEventObject()
		{
			stopSendingEvents();
		}

		void startSendingEvents()
		{
			stopSendingEvents();
			invokeStuff = std::thread([this]()
			{
				while (run)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(eventDelay));
					invokeFunc();
				}
			});
		}

		void stopSendingEvents()
		{
			if (invokeStuff.joinable())
			{
				run = false;
				invokeStuff.join();
			}
		}

		size_t getInvokeCount() const
		{
			return invokeCallCount.load();
		}

		obelisk::AsyncEvent<Args...> testEvent;

	private:

		void invokeHandlers(Args...args)
		{
			if (allowDuplicates)
				testEvent.asyncInvokeHandlers_AllowDuplicateEvents(args...);
			else
				testEvent.asyncInvokeHandlers(args...);

			++invokeCallCount;
		}

		bool run = true;
		std::function<void(void)> invokeFunc;
		std::thread invokeStuff;
		std::atomic<size_t> invokeCallCount;
	};

	template <typename Arg, typename...Args>
	void printArgsToStream(std::wstringstream &ss, Arg arg, Args...args)
	{
		printArgsToStream(ss, arg);
		ss << ", ";
		printArgsToStream(ss, args...);
	}

	template <typename Arg>
	void printArgsToStream(std::wstringstream &ss, Arg arg)
	{
		ss << "(" << typeid(arg).name() << ") " << arg;
	}

	void printArgsToStream(std::wstringstream &ss)
	{
		// Dummy function
	}	
}