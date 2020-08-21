#include "catch.hpp"

#include "obelisk/FormatString.hpp"
#include "obelisk/StringFuncs.h"

using namespace obelisk;

TEST_CASE("TestDurationTimeFormat")
{
	SECTION("10ms duration format")
	{
		std::chrono::duration<double> milliseconds10(10.0 / 1000.0);
		REQUIRE(std::string("10ms") == formatTime(milliseconds10, true));
		REQUIRE(std::string("0s") == formatTime(milliseconds10, false));
	}

	SECTION("1s5ms duration format")
	{
		std::chrono::duration<double> oneSecondFiveMilliseconds(1 + 5.0 / 1000.0);
		REQUIRE(std::string("1s 5ms") == formatTime(oneSecondFiveMilliseconds, true));
		REQUIRE(std::string("1s") == formatTime(oneSecondFiveMilliseconds, false));
	}

	SECTION("1s0ms duration format")
	{
		std::chrono::duration<double> oneSecondZeroMilliseconds(1.0);
		REQUIRE(std::string("1s 0ms") == formatTime(oneSecondZeroMilliseconds, true));
		REQUIRE(std::string("1s") == formatTime(oneSecondZeroMilliseconds, false));
	}

	SECTION("1m 5s 200ms duration format")
	{
		std::chrono::duration<double> oneMin5s200ms(60 + 5 + 200.0 / 1000.0);
		REQUIRE(std::string("1m 5s 200ms") == formatTime(oneMin5s200ms, true));
		REQUIRE(std::string("1m 5s") == formatTime(oneMin5s200ms, false));
	}

	SECTION("2h1m5s200ms duration format")
	{
		std::chrono::duration<double> twoHourOneMin5s200ms(2 * 60 * 60 + 60 + 5 + 200.0 / 1000.0);
		REQUIRE(std::string("2h 1m 5s 200ms") == formatTime(twoHourOneMin5s200ms, true));
		REQUIRE(std::string("2h 1m 5s") == formatTime(twoHourOneMin5s200ms, false));
	}

	SECTION("3d 2h1m5s200ms duration format")
	{
		std::chrono::duration<double> threeDayTwoHourOneMin5s200ms(3 * 24 * 60 * 60 + 2 * 60 * 60 + 60 + 5 + 200.0 / 1000.0);
		REQUIRE(std::string("3d 2h 1m 5s 200ms") == formatTime(threeDayTwoHourOneMin5s200ms, true));
		REQUIRE(std::string("3d 2h 1m 5s") == formatTime(threeDayTwoHourOneMin5s200ms, false));
	}
}

TEST_CASE("FormatString_Simple")
{
	SECTION("std::string") { REQUIRE(std::string{"abc123"} == formatString("%s%d", "abc", 123)); }
	SECTION("std::string %S") { REQUIRE(std::string{"abc123"} == formatString("%S%d", "abc", 123)); }

	SECTION("std::wstring number only") { REQUIRE(std::wstring{L"12"} == formatString(L"%d", 12)); }
	SECTION("std::wstring") { REQUIRE(std::wstring{L"abc123"} == formatString(L"%S%d", L"abc", 123)); }
	SECTION("std::wstring %s") { REQUIRE(std::wstring{L"abc123"} == formatString(L"%s%d", L"abc", 123)); }
}

TEST_CASE("FormatString_LargeArg")
{
	SECTION("std::string")
	{
		std::string a;
		for (size_t i = 0; i < 10000; ++i)
			a += std::to_string(i);
		REQUIRE(a == formatString("%s", a.c_str()));
	}

	SECTION("std::wstring")
	{
		std::wstring b;
		for (size_t i = 0; i < 10000; ++i)
			b += std::to_wstring(i);
		REQUIRE(b == formatString(L"%S", b.c_str()));
	}

	// Below should not compile
	// formatString(L"", a.c_str());
	// formatString("", b.c_str());
	// formatString("", a);
	// formatString(L"", b);
}

TEST_CASE("FormatString_Empty")
{
	SECTION("std::string")
	{
		std::string a;
		REQUIRE(a == formatString("%s", a.c_str()));
	}

	SECTION("std::wstring")
	{
		std::wstring b;
		REQUIRE(b == formatString(L"%s", b.c_str()));
	}
}
