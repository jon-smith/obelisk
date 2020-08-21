#include "catch.hpp"

#include "obelisk/LockUtilsT.hpp"

#include <memory>
#include <string>

using namespace obelisk;

// N.B. These tests don't test any thread safety

TEST_CASE("LockAndCall")
{
	Mutex m;

	bool called = false;

	lockAndCall(m, [&called]() { called = true; });

	REQUIRE(called == true);
}