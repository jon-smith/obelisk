#include "catch.hpp"

#include "obelisk/ArrayParamCalc.hpp"

using namespace obelisk;

TEST_CASE("FindMinMaxTest")
{
	std::vector<int> const values = {2, 1, 4, 3};
	const auto [min, max] = findMinAndMax(std::begin(values), std::end(values));

	REQUIRE(min == 1);
	REQUIRE(max == 4);
}
