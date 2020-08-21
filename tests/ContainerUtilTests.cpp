#include "catch.hpp"

#include "obelisk/ContainerUtils.hpp"
#include "obelisk/StringFuncs.h"

using namespace obelisk;

TEST_CASE("ConvertAllTest")
{
	std::vector<int> const values = {1, 2, 3, 4};
	const auto doubleValues = convertAll<double>(values, [](const int i) { return static_cast<double>(i); });

	REQUIRE(values.size() == doubleValues.size());
	for (size_t i = 0; i < values.size(); ++i)
	{
		REQUIRE(static_cast<double>(values[i]) == doubleValues[i]);
	}
}

TEST_CASE("RestrictIncreasingRange")
{
	SECTION("Increasing vector should not be reduced")
	{
		std::vector<int> const increasing = {1, 2, 3, 4, 5};
		REQUIRE(increasing == restrictToIncreasingRange(2, increasing));
	}

	SECTION("Decreasing vector should be reduced to single item")
	{
		std::vector<int> const decreasing = {5, 4, 3, 2, 1};
		REQUIRE(std::vector<int>(1, decreasing[2]) == restrictToIncreasingRange(2, decreasing));
	}

	SECTION("Vector is not restricted")
	{
		std::vector<int> const increasingInMiddle = {5, 4, 3, 2, 1, 2, 3, 4, 5, 4, 3, 4, 5};
		REQUIRE(std::vector<int>({1, 2, 3, 4, 5}) == restrictToIncreasingRange(4, increasingInMiddle));
	}

	SECTION("Flat should be restricted to single item")
	{
		std::vector<int> flat = {1, 1, 1};
		REQUIRE(std::vector<int>(1, flat[1]) == restrictToIncreasingRange(1, flat));
	}
}

TEST_CASE("RestrictDecreasingRange")
{
	SECTION("Increasing vector should be reduced to a single item")
	{
		std::vector<int> const increasing = {1, 2, 3, 4, 5};
		REQUIRE(std::vector<int>(1, increasing[2]) == restrictToDecreasingRange(2, increasing));
	}

	SECTION("Decreasing vector should not be reduced")
	{
		std::vector<int> const decreasing = {5, 4, 3, 2, 1};
		REQUIRE(decreasing == restrictToDecreasingRange(2, decreasing));
	}

	SECTION("Vector is not restricted")
	{
		std::vector<int> const decreasingInMiddle = {5, 4, 3, 2, 1, 2, 3, 4, 5, 4, 3, 4, 5};
		REQUIRE(std::vector<int>({5, 4, 3, 2, 1}) == restrictToDecreasingRange(4, decreasingInMiddle));
	}

	SECTION("Flat should be restricted to single item")
	{
		std::vector<int> flat = {1, 1, 1};
		REQUIRE(std::vector<int>(1, flat[1]) == restrictToDecreasingRange(1, flat));
	}
}

TEST_CASE("RestrictIncreasingRangeCustomComparator")
{
	std::vector<std::pair<int, int>> const decreasingOnFirstIncreasingOnSecond = {{5, 1}, {4, 2}, {3, 3}, {2, 4}, {1, 5}};

	SECTION("Increasing vector should not be reduced")
	{
		// Compare on second (should all increase)
		REQUIRE(decreasingOnFirstIncreasingOnSecond ==
				restrictToIncreasingRange(2, decreasingOnFirstIncreasingOnSecond,
										  [](std::pair<int, int> l, std::pair<int, int> r) { return l.second < r.second; }));
	}

	SECTION("Decreasing vector should be reduced to a single item")
	{
		// Compare on first - should only keep from item
		REQUIRE(
			std::vector<std::pair<int, int>>(1, decreasingOnFirstIncreasingOnSecond[2]) ==
			restrictToIncreasingRange(2, decreasingOnFirstIncreasingOnSecond, [](std::pair<int, int> l, std::pair<int, int> r) { return l.first < r.first; }));
	}
}

TEST_CASE("RestrictRangeThrows")
{
	const std::vector<int> vec = {1, 1, 1, 1};

	SECTION("Increasing out of range") { REQUIRE_THROWS_AS(restrictToIncreasingRange(vec.size(), vec), std::invalid_argument); }

	SECTION("Increasing empty") { REQUIRE_THROWS_AS(restrictToIncreasingRange(0, std::vector<int>()), std::invalid_argument); }

	SECTION("Decreasing out of range") { REQUIRE_THROWS_AS(restrictToDecreasingRange(vec.size(), vec), std::invalid_argument); }

	SECTION("Decreasing empty") { REQUIRE_THROWS_AS(restrictToDecreasingRange(0, std::vector<int>()), std::invalid_argument); }
}
