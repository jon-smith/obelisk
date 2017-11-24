#pragma once

namespace obelisk_exp
{
	// Runs multiple futures simultaneously that sleep for an amount of time
	void multiConcurrentStdFutureTimings(unsigned min, unsigned multiplier, unsigned count);

	// Same as above, but the a first set of future wait on the second set of futures to start
	void multiConcurrentDualDependanceStdFutureTimings(unsigned min, unsigned multiplier, unsigned count);

	// Runs multiple threads simultaneously that sleep for an amount of time
	void multiConcurrentStdThreadTimings(unsigned min, unsigned multiplier, unsigned count);
}