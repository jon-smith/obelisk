#include "catch.hpp"

#include "obelisk/SortedIterable.hpp"

using namespace obelisk;

TEST_CASE("TestSortInts")
{
	const auto sourceInts = std::vector<int>{4,5,1,3,2};
	const auto sortFunc = std::function<bool(const int &, const int &)>([]( int a,  int b){return a<b;});
	std::vector<int> resultOrder;
	for (const auto &i : iterateSorted(sourceInts, sortFunc))
	{
		resultOrder.push_back(i);
	}

	REQUIRE(resultOrder == std::vector<int>({01,2,3,4,5}));
}

struct TestXY{
	double x={};
	double y={};
	TestXY() = default;
	TestXY(double x, double y):x(x),y(y){};
	TestXY(TestXY &&o):x(o.x), y(o.y){};

	// Prevent copy to confirm no copies are made
	TestXY(const TestXY &) = delete;
};

// Helper function to construct with move only type
std::vector<TestXY> makeTestVec(const std::vector<double> &xs)
{
	std::vector<TestXY> xys;
	for (const auto &x : xs)
	{
		xys.emplace_back(x, 0);
	}
	return xys;
}

TEST_CASE("TestSortStructs")
{
	const auto xys = makeTestVec({3,5,2});
	std::vector<double> sortedXs;
	for (const auto &xy : iterateSorted(xys, [](const TestXY &a, const TestXY &b){return a.x < b.x;}))
	{
		sortedXs.push_back(xy.x);
	}

	REQUIRE(sortedXs == std::vector<double>({2,3,5}));
}

TEST_CASE("TestSortStructsOn")
{
	const auto xys = makeTestVec({3,5,2});
	std::vector<double> sortedXs;
	for (const auto &xy : iterateSortedOn(xys, [](const TestXY &a){return a.x;}))
	{
		sortedXs.push_back(xy.x);
	}

	REQUIRE(sortedXs == std::vector<double>({2,3,5}));
}

TEST_CASE("TestSortMutableStructs")
{
	auto xys = makeTestVec({3,5,2});
	std::vector<double> sortedXs;
	for (auto &xy : iterateSorted(xys, [](const TestXY &a, const TestXY &b){return a.x < b.x;}))
	{
		sortedXs.push_back(xy.x);
		// Set y=x
		xy.y = xy.x;
	}

	REQUIRE(sortedXs == std::vector<double>({2,3,5}));

	std::vector<double> unsortedYs;
	for (auto &xy : xys)
	{
		unsortedYs.push_back(xy.y);
	}

	REQUIRE(unsortedYs == std::vector<double>({3,5,2}));
}

TEST_CASE("TestSortMutableStructsOn")
{
	auto xys = makeTestVec({3,5,2});
	std::vector<double> sortedXs;
	for (auto &xy : iterateSortedOn(xys, [](const TestXY &a){return a.x;}))
	{
		sortedXs.push_back(xy.x);
		// Set y=x
		xy.y = xy.x;
	}

	REQUIRE(sortedXs == std::vector<double>({2,3,5}));

	std::vector<double> unsortedYs;
	for (auto &xy : xys)
	{
		unsortedYs.push_back(xy.y);
	}

	REQUIRE(unsortedYs == std::vector<double>({3,5,2}));
}
