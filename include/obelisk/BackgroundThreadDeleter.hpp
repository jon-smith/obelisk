#pragma once
#include <memory>
#include <mutex>
#include <thread>

namespace obelisk
{
// Class which deletes an object in another thread
template <typename T> class BackgroundThreadDeleter
{
  public:
	static void DeleteObject(std::unique_ptr<T> &&object)
	{
		// We can call new here, as the destructor will be called in a thread
		new BackgroundThreadDeleter<T>(std::move(object));
	}

  private:
	BackgroundThreadDeleter() = delete;
	BackgroundThreadDeleter(std::unique_ptr<T> &&object) : toDelete(std::move(object))
	{
		// Hold a mutex to prevent the thread function running
		std::lock_guard<std::mutex> lock(mutex);

		deleter = std::thread([this] {
			// Wait for the mutex to unblock before continuing
			{
				std::lock_guard<std::mutex> lock(mutex);
			}
			this->~BackgroundThreadDeleter();
		});

		// Detach so the destructor doesn't have issues trying to clean up deleter
		deleter.detach();
	}
	~BackgroundThreadDeleter() = default;
	std::thread deleter;
	std::unique_ptr<T> toDelete;
	std::mutex mutex;
};
} // namespace obelisk