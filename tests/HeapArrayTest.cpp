#include "catch.hpp"

#include "obelisk/HeapArray.hpp"
#include "obelisk/StringFuncs.h"

using namespace obelisk;

TEST_CASE("HeapArray_DefaultInit")
{
	HeapArray<double, 10> tenDoubles;
	for (auto const &t : tenDoubles)
		REQUIRE(t == double{});
}

TEST_CASE("HeapArray_ListInitConstructFull")
{
	HeapArray<double, 5> fiveDoubles{1.0, 1.0, 1.0, 1.0, 1.0};
	for (auto const &t : fiveDoubles)
		REQUIRE(t == 1.0);
}

TEST_CASE("HeapArray_ListInitConstructHalf")
{
	HeapArray<double, 4> fourDoubles{1.0, 1.0};
	for (size_t i = 0; i < fourDoubles.size(); ++i)
	{
		if (i < 2)
			REQUIRE(fourDoubles[i] == 1.0);
		else
			REQUIRE(fourDoubles[i] == double{});
	}
}

TEST_CASE("HeapArray_Copy")
{
	HeapArray<double, 2> twoDoubles{1.0, 1.0};
	auto const anotherTwoDoubles = twoDoubles;

	for (size_t i = 0; i < twoDoubles.size(); ++i)
		REQUIRE(twoDoubles[i] == anotherTwoDoubles[i]);
}

TEST_CASE("HeapArray_Fill")
{
	HeapArray<double, 2> twoDoubles;

	twoDoubles.fill(1.0);

	for (auto const &t : twoDoubles)
		REQUIRE(t == 1.0);
}

TEST_CASE("HeapArray_OutOfRangeConstAt")
{
	HeapArray<double, 4> fourDoubles;

	REQUIRE_THROWS_AS(
		[&fourDoubles]() {
			double sum = 0;
			for (size_t i = 0; i < 5; ++i)
				sum += fourDoubles.at(i);
		}(),
		std::out_of_range);
}

TEST_CASE("HeapArray_OutOfRangeMutableAt")
{
	HeapArray<double, 4> fourDoubles;

	REQUIRE_THROWS_AS(
		[&fourDoubles]() {
			for (size_t i = 0; i < 5; ++i)
				fourDoubles.at(i) += 1.0;
		}(),
		std::out_of_range);
}
