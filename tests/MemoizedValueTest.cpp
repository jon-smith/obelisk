#include "catch.hpp"

#include "obelisk/FormatString.hpp"
#include "obelisk/MemoizedValue.hpp"
#include "obelisk/StringFuncs.h"

using namespace obelisk;

TEST_CASE("MemoizedValueDouble")
{
	unsigned callCount = 0;
	auto const lambdaCalc = [&callCount]() {
		++callCount;
		return 10.0;
	};

	const auto lambdaResult = lambdaCalc();
	callCount = 0;

	MemoizedValue<double> m(lambdaCalc);
	REQUIRE(callCount == 0);

	// First result calls once
	const auto firstMemoResult = m.get();
	REQUIRE(lambdaResult == firstMemoResult);
	REQUIRE(callCount == 1);

	// Second result retrieves memoized value
	const auto secondMemoResult = m.get();
	REQUIRE(lambdaResult == secondMemoResult);
	REQUIRE(callCount == 1);

	// Reset causes reeval
	m.reset();
	const auto postResetMemoResult = m.get();
	REQUIRE(lambdaResult == postResetMemoResult);
	REQUIRE(callCount == 2);
}
