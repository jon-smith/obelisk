#pragma once

// C++ standard thread support library wrappers
// Most of these are incompatible with C++/cli, such as <mutex> and <thread>

#include <memory>
#include <chrono>

namespace obelisk
{
	class ConditionVariable;

	class Mutex
	{
	public:
		Mutex();
		~Mutex();
		void lock();
		void unlock();
		bool tryLock();
	protected:
		struct Impl;
		const std::unique_ptr<Impl> m;
		friend class ConditionVariable;
	};

	class LockGuard
	{
	public:
		LockGuard(Mutex & m) :m(m) { m.lock(); }
		~LockGuard() { m.unlock(); }
	private:
		Mutex&m;
	};

	class SharedMutex
	{
	public:
		SharedMutex();
		~SharedMutex();
		void lock();
		void lockShared();
		void unlock();
		void unlockShared();
		bool tryLock();
		bool tryLockShared();
	protected:
		struct Impl;
		const std::unique_ptr<Impl> m;
	};

	template <typename MutexType>
	class UniqueLock
	{
	public:
		static_assert(std::is_same<MutexType, Mutex>::value || std::is_same<MutexType, SharedMutex>::value, "Only supports obelisk mutex types");
		UniqueLock() = delete;
		UniqueLock(MutexType &m) :m(m) { m.lock(); }
		~UniqueLock() { if (owns) m.unlock(); }
		void unlock() { if (owns) m.unlock(); owns = false; }
	private:
		void release() { owns = false; };
		bool owns = true;
		MutexType&m;
		friend class ConditionVariable;
	};

	class SharedLock
	{
	public:
		SharedLock() = delete;
		SharedLock(SharedMutex &m) :m(m) { m.lockShared(); }
		~SharedLock() { m.unlockShared(); }
	private:
		SharedMutex &m;
	};

	class ConditionVariable
	{
	public:
		ConditionVariable();
		~ConditionVariable();

		void notifyOne();

		enum class Status { TIMEOUT, NO_TIMEOUT };
		Status waitFor(UniqueLock<Mutex> &lk, std::chrono::milliseconds timeout);
		void wait(UniqueLock<Mutex> &lk);

	protected:
		struct Impl;
		const std::unique_ptr<Impl> cv;
	};
}