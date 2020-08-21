#include "TestHelpers.hpp"
#include "catch.hpp"

#include "obelisk/BackgroundThreadDeleter.hpp"

#include <chrono>

using namespace obelisk;

class TrackDestruction
{
  public:
	TrackDestruction(bool &isDestroyed) : isDestroyed(isDestroyed) { isDestroyed = false; }
	~TrackDestruction() { isDestroyed = true; }

  private:
	bool &isDestroyed;
};

TEST_CASE("BackgroundThreadDeleterSmokeTest")
{
	bool isDestroyed = false;
	auto tracker = std::make_unique<TrackDestruction>(isDestroyed);
	BackgroundThreadDeleter<TrackDestruction>::DeleteObject(std::move(tracker));

	WaitForTrue(std::chrono::seconds(5), [&isDestroyed]() { return isDestroyed; });

	REQUIRE(isDestroyed);
}