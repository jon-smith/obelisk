#include "catch.hpp"

#include "obelisk/SimpleAsyncTask.h"

using namespace obelisk;

TEST_CASE("SimpleAsyncTaskWait")
{
	bool called = false;
	SimpleAsyncTask task([&called]() { called = true; });

	task.wait();
	REQUIRE(called);
}

TEST_CASE("SimpleAsyncTaskEndOfScope")
{
	bool called = false;
	{
		SimpleAsyncTask task([&called]() { called = true; });
		// wait() called in destructor
	}
	REQUIRE(called);
}