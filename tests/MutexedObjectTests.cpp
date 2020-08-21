#include "catch.hpp"

#include "obelisk/MutexedObject.hpp"
#include "obelisk/StringFuncs.h"

#include <memory>
#include <string>

using namespace obelisk;

// N.B. These tests don't test any thread safety

TEST_CASE("SimpleObject")
{

	auto integer = MutexedObject<int>(0);
	REQUIRE(integer.get() == 0);

	integer.set(1);
	REQUIRE(integer.get() == 1);

	integer.lockedModify([](auto &i) { i = 0; });
	REQUIRE(integer.get() == 0);
	integer.lockedAccess([](auto &i) { REQUIRE(i == 0); });

	const auto result = integer.lockedAccess<int>([](auto &i) { return i; });
	REQUIRE(result == 0);
}