#include "catch.hpp"

#include "obelisk/ContainerMaker.hpp"

using namespace obelisk;

TEST_CASE("TestGenerateVector0to9")
{
	auto const vZeroToNine = generateVector<size_t>([](size_t idx) { return idx; }, 0, 10);
	REQUIRE(vZeroToNine == std::vector<size_t>({0, 1, 2, 3, 4, 5, 6, 7, 8, 9}));
}

TEST_CASE("TestGenerateVector2to9")
{
	auto const vTwoToNine = generateVector<size_t>([](size_t idx) { return idx; }, 2, 8);
	REQUIRE(vTwoToNine == std::vector<size_t>({2, 3, 4, 5, 6, 7, 8, 9}));
}
