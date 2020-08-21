#include "catch.hpp"

#include "obelisk/MultiVector.hpp"
#include "obelisk/StringFuncs.h"

#include <memory>
#include <string>

using namespace obelisk;

// Just test that they construct
TEST_CASE("MultiVector_DefaultConstruct")
{
	const auto threeVecsDefault = MultiVector<double, int, std::string>();
	REQUIRE(size_t(0) == threeVecsDefault.size());
	REQUIRE(threeVecsDefault.isEmpty());
}

TEST_CASE("MultiVector_CopyConstruct")
{
	std::vector<double> dV(5);
	std::vector<int> iV(5);
	std::vector<std::string> strV(5);

	const auto threeVecsCopied = MakeMultiVector(dV, iV, strV);
	const auto threeVecsCopied2 = MultiVector<double, int, std::string>(dV, iV, strV);

	// Sizes
	REQUIRE(size_t(5) == threeVecsCopied.size());
	REQUIRE(size_t(5) == threeVecsCopied2.size());
	// Empty
	REQUIRE(!threeVecsCopied.isEmpty());
	REQUIRE(!threeVecsCopied2.isEmpty());
	// Contents
	REQUIRE(dV == threeVecsCopied.get<0>());
	REQUIRE(iV == threeVecsCopied.get<1>());
	REQUIRE(strV == threeVecsCopied.get<2>());
	REQUIRE(dV == threeVecsCopied2.get<0>());
	REQUIRE(iV == threeVecsCopied2.get<1>());
	REQUIRE(strV == threeVecsCopied2.get<2>());
}

TEST_CASE("MultiVector_MoveConstruct")
{
	// Create multivectors with a move-only type
	const auto twoVecs = MakeMultiVector(std::vector<int>(2), std::vector<std::unique_ptr<int>>(2));
	const auto twoVecs2 = MultiVector<int, std::unique_ptr<int>>(std::vector<int>(2), std::vector<std::unique_ptr<int>>(2));

	// Sizes
	REQUIRE(size_t(2) == twoVecs.size());
	REQUIRE(size_t(2) == twoVecs2.size());
	// Empty
	REQUIRE(!twoVecs.isEmpty());
	REQUIRE(!twoVecs2.isEmpty());
	// Contents
	REQUIRE(std::vector<int>(2) == twoVecs.get<0>());
	REQUIRE(std::vector<std::unique_ptr<int>>(2) == twoVecs.get<1>());
	REQUIRE(std::vector<int>(2) == twoVecs2.get<0>());
	REQUIRE(std::vector<std::unique_ptr<int>>(2) == twoVecs2.get<1>());
}

TEST_CASE("MultiVector_RefConstruct")
{
	std::vector<double> dV(5);
	std::vector<int> iV(5);
	std::vector<std::string> strV(5);

	const auto threeVecRefs = MakeMultiVectorRef(dV, iV, strV);
	const auto threeVecRefs2 = MultiVectorRef<double, int, std::string>(dV, iV, strV);

	// Test contents identical
	REQUIRE(size_t(5) == threeVecRefs.size());
	REQUIRE(size_t(5) == threeVecRefs2.size());
	REQUIRE(!threeVecRefs.isEmpty());
	REQUIRE(!threeVecRefs2.isEmpty());
	// Contents
	REQUIRE(dV == threeVecRefs.get<0>());
	REQUIRE(iV == threeVecRefs.get<1>());
	REQUIRE(strV == threeVecRefs.get<2>());
	REQUIRE(dV == threeVecRefs2.get<0>());
	REQUIRE(iV == threeVecRefs2.get<1>());
	REQUIRE(strV == threeVecRefs2.get<2>());
}

TEST_CASE("MultiVector_CRefConstruct")
{
	const std::vector<double> dV(5);
	const std::vector<int> iV(5);
	const std::vector<std::string> strV(5);

	const auto threeVecRefs = MakeMultiVectorRef(dV, iV, strV);
	static_assert(std::is_same_v<std::decay_t<decltype(threeVecRefs)>, MultiVectorCRef<double, int, std::string>>, "Make should construct CRef version");

	const auto threeVecRefs2 = MultiVectorCRef<double, int, std::string>(dV, iV, strV);

	// Test contents identical
	REQUIRE(size_t(5) == threeVecRefs.size());
	REQUIRE(size_t(5) == threeVecRefs2.size());
	REQUIRE(!threeVecRefs.isEmpty());
	REQUIRE(!threeVecRefs2.isEmpty());
	// Contents
	REQUIRE(dV == threeVecRefs.get<0>());
	REQUIRE(iV == threeVecRefs.get<1>());
	REQUIRE(strV == threeVecRefs.get<2>());
	REQUIRE(dV == threeVecRefs2.get<0>());
	REQUIRE(iV == threeVecRefs2.get<1>());
	REQUIRE(strV == threeVecRefs2.get<2>());
}

TEST_CASE("MultiVector_SizeConstruct")
{
	const auto threeVecsSize3 = MultiVector<double, int, std::string>(3);

	// Test size 3 with all default init
	REQUIRE(size_t(3) == threeVecsSize3.size());
	REQUIRE(!threeVecsSize3.isEmpty());
}

TEST_CASE("MultiVector_ThrowOnInconsistentSizes")
{
	std::vector<double> dV(5);
	std::vector<int> dI(4);

	REQUIRE_THROWS_AS(MakeMultiVector(dV, dI), std::invalid_argument);

	REQUIRE_THROWS_AS(MakeMultiVectorRef(dV, dI), std::invalid_argument);

	REQUIRE_THROWS_AS(MakeMultiVectorCRef(dV, dI), std::invalid_argument);

	auto constructMV = [&dV, &dI]() { MultiVector<double, int>(dV, dI); };
	REQUIRE_THROWS_AS(constructMV(), std::invalid_argument);

	auto constructMVR = [&dV, &dI]() { MultiVectorRef<double, int>(dV, dI); };
	REQUIRE_THROWS_AS(constructMVR(), std::invalid_argument);

	auto constructMVCR = [&dV, &dI]() { MultiVectorCRef<double, int>(dV, dI); };
	REQUIRE_THROWS_AS(constructMVCR(), std::invalid_argument);
}
