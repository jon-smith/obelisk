#include "catch.hpp"

#include "obelisk/ColourStructs.hpp"

using namespace obelisk;

TEST_CASE("ConvertColourSmokeTest")
{
	const ColourHSV hsv(0.5, 0.6, 0.7);
	const auto rgb = toRGB(hsv);
}