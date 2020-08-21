#include "catch.hpp"

#include "obelisk/BackgroundContainerProducer.hpp"

using namespace obelisk;

TEST_CASE("BackgroundContainerProducerConstructEmpty")
{
	SECTION("GetEmpty")
	{
		auto producer = BackgroundContainerProducer<std::vector<double>>();
		REQUIRE(producer.getContainer(0).size() == 0);
	}

	SECTION("GetSized")
	{
		auto producer = BackgroundContainerProducer<std::vector<double>>();
		REQUIRE(producer.getContainer(2).size() == 2);
	}
}

TEST_CASE("BackgroundContainerProducerConstructSized")
{
	SECTION("GetEmpty")
	{
		auto producer = BackgroundContainerProducer<std::vector<double>>(2);
		REQUIRE(producer.getContainer(0).size() == 0);
	}

	SECTION("GetSized")
	{
		auto producer = BackgroundContainerProducer<std::vector<double>>(2);
		REQUIRE(producer.getContainer(2).size() == 2);
	}
}