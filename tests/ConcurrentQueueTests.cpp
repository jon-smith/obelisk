#include "catch.hpp"

#include "obelisk/ConcurrentQueueT.hpp"

#include <memory>
#include <string>

using namespace obelisk;

// N.B. These tests don't test any thread safety

TEST_CASE("SimpleConcurrentQueueSmokeTest")
{
	SimpleConcurrentQueue<int> queue;
	REQUIRE(queue.empty());
	REQUIRE(queue.size() == 0);
	REQUIRE(queue.pop() == int{});

	queue.push(1);
	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 1);
	REQUIRE(queue.pop() == 1);
	REQUIRE(queue.size() == 0);
}

TEST_CASE("MultipleProducerSingleConsumerQueueSmokeTest")
{
	MultipleProducerSingleConsumerQueue<int> queue;
	REQUIRE(queue.empty());
	REQUIRE(queue.size() == 0);
	REQUIRE(queue.waitForPopTimed(std::chrono::milliseconds(10)).second == WaitResult::TIMEOUT);

	queue.push(1);
	REQUIRE(!queue.empty());
	REQUIRE(queue.size() == 1);
	const auto [value, type] = queue.waitForPopTimed(std::chrono::milliseconds(10));
	REQUIRE(value == 1);
	REQUIRE(type == WaitResult::SUCCESS);
	REQUIRE(queue.size() == 0);
}