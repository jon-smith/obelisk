#include <chrono>
#include <functional>
#include <thread>

inline bool WaitForTrue(std::chrono::seconds waitTime, std::function<bool(void)> check)
{
	const auto start = std::chrono::steady_clock::now();
	while (std::chrono::steady_clock::now() - start < waitTime)
	{
		if (check())
			return true;

		std::this_thread::sleep_for(std::chrono::milliseconds(50));
	}

	return check();
}