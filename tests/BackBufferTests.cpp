#include "TestHelpers.hpp"
#include "catch.hpp"

#include "obelisk/BackBuffer.hpp"

#include <chrono>

using namespace obelisk;

TEST_CASE("BackBufferSmokeTest")
{
	auto updateFn = [](std::vector<double> &d, const std::atomic<bool> &kill) {
		if (!d.empty())
			d.clear();
		else
			d = {1.0, 2.0, 3.0};
	};

	BackBufferT<std::vector<double>> backBuffer(updateFn, {1.0, 2.0, 3.0});

	const auto first = backBuffer.getLatestData();
	REQUIRE(first == {1.0, 2.0, 3.0})
	backBuffer.requestUpdate();
	WaitForTrue(std::chrono::seconds(5), [&backBuffer]() { return !backBuffer.isUpdating(); });

	const auto updated = backBuffer.getLatestData();
	REQUIRE(updated.empty());
}

TEST_CASE("BackBufferContinuousSmokeTest")
{
	auto updateFn = [](std::vector<double> &d, const std::atomic<bool> &kill) {
		if (!d.empty())
			d.clear();
	};

	BackBufferT<std::vector<double>> backBuffer(updateFn, {1.0, 2.0, 3.0}, true);
	const auto isEmpty = WaitForTrue(std::chrono::seconds(5), [&backBuffer]() { return backBuffer.getLatestData().empty(); });
	REQUIRE(isEmpty);
}