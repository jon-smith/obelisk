#include "stdafx.h"
#include "CppUnitTest.h"

#include "StringFuncs.h"
#include "FormatString.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
  TEST_CLASS(StringFuncTests)
  {
  public:

    TEST_METHOD(TestDurationTimeFormatWithMs)
    {
      std::chrono::duration<double> milliseconds10(10.0 / 1000.0);
      Assert::AreEqual(std::string("10ms"), formatTime(milliseconds10, true), L"Failed 10ms duration format");

      std::chrono::duration<double> oneSecondFiveMilliseconds(1 + 5.0 / 1000.0);
      Assert::AreEqual(std::string("1s 5ms"), formatTime(oneSecondFiveMilliseconds, true), L"Failed 1s5ms duration format");

      std::chrono::duration<double> oneSecondZeroMilliseconds(1.0);
      Assert::AreEqual(std::string("1s 0ms"), formatTime(oneSecondZeroMilliseconds, true), L"Failed 1s0ms duration format");

      std::chrono::duration<double> oneMin5s200ms(60 + 5 + 200.0 / 1000.0);
      Assert::AreEqual(std::string("1m 5s 200ms"), formatTime(oneMin5s200ms, true), L"Failed 1m5s200ms duration format");

      std::chrono::duration<double> twoHourOneMin5s200ms(2 *60*60 + 60 + 5 + 200.0 / 1000.0);
      Assert::AreEqual(std::string("2h 1m 5s 200ms"), formatTime(twoHourOneMin5s200ms, true), L"Failed 2h1m5s200ms duration format");

      std::chrono::duration<double> threeDayTwoHourOneMin5s200ms(3*24*60*60 + 2 * 60 * 60 + 60 + 5 + 200.0 / 1000.0);
      Assert::AreEqual(std::string("3d 2h 1m 5s 200ms"), formatTime(threeDayTwoHourOneMin5s200ms, true), L"Failed 3d 2h1m5s200ms duration format");
    }    

    TEST_METHOD(TestDurationTimeFormatWithoutMs)
    {
      std::chrono::duration<double> milliseconds10(10.0 / 1000.0);
      Assert::AreEqual(std::string("0s"), formatTime(milliseconds10, false), L"Failed 0s duration format");

      std::chrono::duration<double> oneSecondFiveMilliseconds(1 + 5.0 / 1000.0);
      Assert::AreEqual(std::string("1s"), formatTime(oneSecondFiveMilliseconds, false), L"Failed 1s duration format");

      std::chrono::duration<double> oneSecondZeroMilliseconds(1.0);
      Assert::AreEqual(std::string("1s"), formatTime(oneSecondZeroMilliseconds, false), L"Failed 1s duration format");

      std::chrono::duration<double> oneMin5s200ms(60 + 5 + 200.0 / 1000.0);
      Assert::AreEqual(std::string("1m 5s"), formatTime(oneMin5s200ms, false), L"Failed 1m5s duration format");

      std::chrono::duration<double> twoHourOneMin5s200ms(2 * 60 * 60 + 60 + 5 + 200.0 / 1000.0);
      Assert::AreEqual(std::string("2h 1m 5s"), formatTime(twoHourOneMin5s200ms, false), L"Failed 2h1m5s duration format");

      std::chrono::duration<double> threeDayTwoHourOneMin5s200ms(3 * 24 * 60 * 60 + 2 * 60 * 60 + 60 + 5 + 200.0 / 1000.0);
      Assert::AreEqual(std::string("3d 2h 1m 5s"), formatTime(threeDayTwoHourOneMin5s200ms, false), L"Failed 3d 2h1m5s duration format");
    }

		TEST_METHOD(FormatString_LargeArg)
		{
			std::string a;
			for (size_t i = 0; i < 10000; ++i)
				a += std::to_string(i);

			Assert::AreEqual(a, formatString("%s", a.c_str()));
						
			std::wstring b;
			for (size_t i = 0; i < 10000; ++i)
				b += std::to_wstring(i);

			Assert::AreEqual(b, formatString(L"%s", b.c_str()));

			// Below should not compile
			//formatString(L"", a.c_str());
			//formatString("", b.c_str());
			//formatString("", a);
			//formatString(L"", b);
		}

		TEST_METHOD(FormatString_Empty)
		{
			std::string a;
			Assert::AreEqual(a, formatString("%s", a.c_str()));

			std::wstring b;
			Assert::AreEqual(b, formatString(L"%s", b.c_str()));
		}
  };
}