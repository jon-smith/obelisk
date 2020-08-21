#include "catch.hpp"

#include "obelisk/StackVector.hpp"
#include "obelisk/StringFuncs.h"

using namespace obelisk;

TEST_CASE("StackVector_DefaultInit")
{
	StackVector<double, 10> vec;
	REQUIRE(vec.size() == size_t{});
}

TEST_CASE("StackVector_ListInitConstructFull")
{
	StackVector<double, 5> fiveDoubles{1.0, 1.0, 1.0, 1.0, 1.0};
	REQUIRE(size_t(5) == fiveDoubles.size());
	size_t count = 0;
	for (auto const &v : fiveDoubles)
	{
		REQUIRE(v == 1.0);
		++count;
	}
	// Should iterate through 5 elements
	REQUIRE(count == size_t{5});
}

TEST_CASE("StackVector_ListInitConstructHalf")
{
	StackVector<double, 4> fourDoubles{1.0, 1.0};
	REQUIRE(size_t(2) == fourDoubles.size());
	for (size_t i = 0; i < 4; ++i)
	{
		if (i < 2)
		{
			// First two values should be 1
			REQUIRE(fourDoubles[i] == 1.0);
		}
		else
		{
			// Other values should be default initialized
			REQUIRE(fourDoubles[i] == double{});
		}
	}
}

TEST_CASE("StackVector_Copy")
{
	StackVector<double, 2> twoDoubles{1.0, 1.0};
	auto const anotherTwoDoubles = twoDoubles;

	REQUIRE(twoDoubles.size() == anotherTwoDoubles.size());

	for (size_t i = 0; i < twoDoubles.size(); ++i)
		REQUIRE(twoDoubles[i] == anotherTwoDoubles[i]);
}

TEST_CASE("StackVector_Assign")
{
	StackVector<double, 2> twoDoubles{1.0, 1.0};
	StackVector<double, 2> anotherTwoDoubles;
	anotherTwoDoubles = twoDoubles;

	REQUIRE(twoDoubles.size() == anotherTwoDoubles.size());

	for (size_t i = 0; i < twoDoubles.size(); ++i)
		REQUIRE(twoDoubles[i] == anotherTwoDoubles[i]);
}

TEST_CASE("StackVector_Fill")
{
	StackVector<double, 5> doubles(5);

	doubles.fill(1.0);

	size_t count = 0;
	for (auto const &v : doubles)
	{
		REQUIRE(v == 1.0);
		++count;
	}
	REQUIRE(count == size_t{5});
}

TEST_CASE("StackVector_PushBack")
{
	StackVector<double, 10> vec;

	for (size_t i = 0; i < 10; ++i)
		vec.push_back(static_cast<double>(i));
	REQUIRE(vec.size() == size_t{10});

	size_t count = 0;
	for (auto const &v : vec)
	{
		REQUIRE(v == static_cast<double>(count));
		++count;
	}

	REQUIRE(count == size_t{10});
}

TEST_CASE("StackVector_EmplaceBack")
{
	StackVector<double, 10> vec;

	for (size_t i = 0; i < 10; ++i)
		vec.emplace_back(static_cast<double>(i));
	REQUIRE(vec.size() == size_t{10});

	size_t count = 0;
	for (auto const &v : vec)
	{
		REQUIRE(v == static_cast<double>(count));
		++count;
	}
	REQUIRE(count == size_t{10});
}

TEST_CASE("StackVector_EmplaceBackUnchecked")
{
	StackVector<double, 10> vec;

	for (size_t i = 0; i < 10; ++i)
		vec.emplace_back_unchecked(static_cast<double>(i));
	REQUIRE(vec.size() == size_t{10});

	size_t count = 0;
	for (auto const &v : vec)
	{
		REQUIRE(v == static_cast<double>(count));
		++count;
	}
	REQUIRE(count == size_t{10});
}

TEST_CASE("StackVector_OutOfRangeConstAt")
{
	StackVector<double, 4> fourDoubles(4);

	auto loopAndInc = [&]() {
		double sum = 0;
		for (size_t i = 0; i < 5; ++i)
		{
			sum += fourDoubles.at(i);
		}
		return sum;
	};

	REQUIRE_THROWS_AS(loopAndInc(), std::out_of_range);
}

TEST_CASE("StackVector_OutOfRangeMutableAt")
{
	StackVector<double, 4> fourDoubles(4);

	auto loopAndInc = [&]() {
		for (size_t i = 0; i < 5; ++i)
		{
			fourDoubles.at(i) += 1.0;
		}
	};

	REQUIRE_THROWS_AS(loopAndInc(), std::out_of_range);
}