#include "stdafx.h"
#include "ThreadLimits.h"

#include <future>
#include <thread>

#include "Logging.h"
#include "FormatString.hpp"
#include "PlatformSpecific.h"

using namespace obelisk;
using namespace obelisk::platform_utilities;

namespace
{
	template <typename FuncT>
	void doTimeAndLogForIndex(obelisk_exp::ExperimentLogger &l, unsigned index, FuncT &&fn)
	{
		auto const timeBefore = std::chrono::high_resolution_clock::now();

		fn();

		auto const timeTaken = std::chrono::high_resolution_clock::now() - timeBefore;

		l.LOG("Timing", obelisk::formatString("%d took %.3f ms", index, static_cast<double>(timeTaken.count()) / 1'000'000));
	}
}

namespace obelisk_exp
{
	void multiConcurrentStdFutureTimings(unsigned min, unsigned multiplier, unsigned count)
	{
		ExperimentLogger l("Conc std future timings");
		l.LOG("Info", "Running multiple concurrent std::futures");
		l.LOG("Info", "Calling the function:");
		l.LOG("Info", "[](){std::this_thread::sleep_for(std::chrono::milliseconds(100));}");
		l.LOG("Info", "Start");

		for (unsigned i = 0; i < count; ++i)
		{
			auto const n = min*static_cast<unsigned>(pow(multiplier, i));

			std::vector<std::future<void>> futures(n);

			doTimeAndLogForIndex(l, n, [&futures]() {
				for (auto &f : futures)
				{
					f = std::async(std::launch::async, []() {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					});
				}

				for (auto &f : futures)
					f.wait();
			});
		}

		l.LOG("Info", "End");
	}

	void multiConcurrentDualDependanceStdFutureTimings(unsigned min, unsigned multiplier, unsigned count)
	{
		ExperimentLogger l("Conc dual depends. std future timings");
		l.LOG("Info", "Running two sets of multiple concurrent std::futures");
		l.LOG("Info", "The first set waits for a flag to be set by the second set.");
		l.LOG("Info", "Start");

		for (unsigned i = 0; i < count; ++i)
		{
			auto const n = min*static_cast<unsigned>(pow(multiplier, i));

			std::vector<std::future<void>> firstFutures(n);
			std::vector<std::future<void>> secondFutures(n);
			std::vector<bool> secondFutureStartedFlag(n, false);

			doTimeAndLogForIndex(l, n, [n, &l, &firstFutures, &secondFutures, &secondFutureStartedFlag]() {

				// Start the first set of futures that wait on the second set starting
				for (size_t f=0; f<n; ++f)
				{
					firstFutures[f] = std::async(std::launch::async, [f, &secondFutureStartedFlag](){
						while (!secondFutureStartedFlag[f])
							// Using std::this_thread::yield doesn't work past 4 threads (on the tested machine)
							std::this_thread::sleep_for(std::chrono::milliseconds(1));
					});
				}

				l.LOG("Process", formatString("Future count: %d, process thread count: %d", n, getCurrentProcessThreadCount()));

				// Start the second set that flag that they have started
				for (size_t f = 0; f < n; ++f)
				{
					secondFutures[f] = std::async(std::launch::async, [f, &secondFutureStartedFlag](){
						secondFutureStartedFlag[f] = true;
					});
				}

				l.LOG("Process", formatString("Future count: %d, process thread count: %d", n*2, getCurrentProcessThreadCount()));


				l.LOG("Process", "Start");

				// Wait for all futures to finish
				for (auto &f : firstFutures)
					f.wait();

				for (auto &f : secondFutures)
					f.wait();
			});
		}

		l.LOG("Info", "End");
	}

	void multiConcurrentStdThreadTimings(unsigned min, unsigned multiplier, unsigned count)
	{
		ExperimentLogger l("Conc std thread timings");
		l.LOG("Info", "Running multiple concurrent std::threads");
		l.LOG("Info", "Calling the function:");
		l.LOG("Info", "[](){std::this_thread::sleep_for(std::chrono::milliseconds(100));}");
		l.LOG("Info", "Start");

		for (unsigned i = 0; i < count; ++i)
		{
			auto const n = min*static_cast<unsigned>(pow(multiplier, i));

			std::vector<std::thread> threads(n);

			doTimeAndLogForIndex(l, n, [&threads]() {
				for (auto &t : threads)
				{
					t = std::thread([]() {
						std::this_thread::sleep_for(std::chrono::milliseconds(100));
					});
				}

				for (auto &t : threads)
					t.join();
			});
		}

		l.LOG("Info", "End");
	}
}