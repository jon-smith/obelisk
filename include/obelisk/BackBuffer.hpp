#pragma once

#include <atomic>
#include <cassert>

#include "obelisk/StdThreadSupportWrappers.h"
#include "obelisk/observer/AsyncEvent.hpp"

namespace obelisk
{
///
/// Class for double buffered data updates/generation
///
/// Data is generated in a background thread which calls the supplied updateFn() function
/// A single update can be requested using requestUpdate()
/// Or continuously updated using setUpdateContinuous(bool)
/// dataUpdate will send an asynchronous event each time the back buffer is updated
///
template <typename T> class BackBufferT : protected SubjectWithEvents
{
  public:
	BackBufferT(std::function<void(T &data, const std::atomic<bool> &killFlag)> updateFn, T initialData, bool updateContinuous = false)
		: dataUpdated(*this, L"BackBuffer Data Updated"), onUpdateRequested(*this, L"BackBuffer Update Requested"), killThread(false), front(initialData),
		  back(initialData), updateRequested(updateContinuous), updateContinuous(updateContinuous), updateFn(updateFn)
	{
		assert(updateFn != nullptr);
		updateThread = std::thread([this] { threadFunc(); });
	}

	BackBufferT(std::function<void(T &data, const std::atomic<bool> &killFlag)> updateFn, bool updateContinuous = false)
		: BackBufferT(updateFn, T{}, updateContinuous){};

	AsyncEvent<> dataUpdated;
	AsyncEvent<> onUpdateRequested;

	T getLatestData() const
	{
		UniqueLock<Mutex> lk(swapOrCopyMutex);
		return front;
	}

	void setUpdateContinuous(bool continuous)
	{
		UniqueLock<Mutex> lk(requestMutex);

		// If we're requesting continuous runs, or changing back to non-continuous
		// Request an update from the update thread
		if (continuous || updateContinuous != continuous)
		{
			updateContinuous = continuous;
			updateRequested = true;
			requestUpdateCv.notifyOne();
			onUpdateRequested.asyncInvokeHandlers();
		}
		// else, we're already non-continuously updating, don't do anything
	}

	void requestUpdate() const
	{
		UniqueLock<Mutex> lk(requestMutex);
		updateRequested = true;
		requestUpdateCv.notifyOne();
		onUpdateRequested.asyncInvokeHandlers();
	}

	bool isUpdating() const
	{
		UniqueLock<Mutex> lk(requestMutex);
		return updateRequested || isUpdatingFlag;
	}

  protected:
	~BackBufferT()
	{
		if (!killThread)
		{
			UniqueLock<Mutex> lk(requestMutex);
			killThread = true;
			lk.unlock();
			requestUpdateCv.notifyOne();
			updateThread.join();
		}
	}

  private:
	mutable Mutex swapOrCopyMutex;
	T front = {};
	T back = {};

	std::atomic<bool> killThread;
	std::thread updateThread;
	std::function<void(T &data, const std::atomic<bool> &killFlag)> updateFn;

	// Request mutex protects the condition variable and updateRequested
	mutable Mutex requestMutex;
	mutable ConditionVariable requestUpdateCv;
	mutable bool updateRequested;
	bool updateContinuous;
	bool isUpdatingFlag;

	void threadFunc()
	{
		while (!killThread)
		{
			// Lock to synchronise with request update and destructor calls
			UniqueLock<Mutex> lk(requestMutex);

			// If we locked after the destructor lock, kill here, as notify_one might have already been called
			if (killThread)
				return;

			// If an update hasn't been requested, wait on one to be requested
			if (!updateRequested)
				requestUpdateCv.wait(lk);

			// The destructor woke the cv, kill here
			if (killThread)
				return;

			// Reset update (if not updating continuously) and unlock
			updateRequested = updateContinuous;
			isUpdatingFlag = true;
			lk.unlock();

			updateFn(back, killThread);

			LockGuard lk2(swapOrCopyMutex);
			std::swap(back, front);
			isUpdatingFlag = false;

			dataUpdated.asyncInvokeHandlers();
		}
	}
};
} // namespace obelisk