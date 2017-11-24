#include "stdafx.h"
#include "CppUnitTest.h"
#include "HeapArray.h"
#include "StringFuncs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
  TEST_CLASS(HeapArrayTest)
  {
  public:

    TEST_METHOD(DefaultInit)
    {
      HeapArray<double, 10> tenDoubles;
      for (auto const &t : tenDoubles)
        Assert::AreEqual(t, double{}, L"All values should be default initialized");
    }

    TEST_METHOD(ListInitConstructFull)
    {
      HeapArray<double, 5> fiveDoubles{ 1.0,1.0,1.0,1.0,1.0 };
      for (auto const &t : fiveDoubles)
        Assert::AreEqual(t, 1.0, L"All values should be 1.0");
    }

    TEST_METHOD(ListInitConstructHalf)
    {
      HeapArray<double, 4> fourDoubles{ 1.0,1.0};
      for (size_t i=0; i<fourDoubles.size(); ++i)
        if (i < 2)
          Assert::AreEqual(fourDoubles[i], 1.0, L"First two values should be 1.0");
        else
          Assert::AreEqual(fourDoubles[i], double{}, L"Other values should be default initialized");
    }

    TEST_METHOD(Copy)
    {
      HeapArray<double, 2> twoDoubles{ 1.0,1.0 };
      auto const anotherTwoDoubles = twoDoubles;

      for (size_t i = 0; i<twoDoubles.size(); ++i)
        Assert::AreEqual(twoDoubles[i], anotherTwoDoubles[i], L"Contents should be identical");       
    }

    TEST_METHOD(Fill)
    {
      HeapArray<double, 2> twoDoubles;

      twoDoubles.fill(1.0);

      for (auto const &t : twoDoubles)
        Assert::AreEqual(t, 1.0, L"All values should be 1.0");     
    }

    TEST_METHOD(OutOfRangeConstAt)
    {
      HeapArray<double, 4> fourDoubles;

      try
      {
        double sum = 0;
        for (size_t i = 0; i < 5; ++i)
          sum += fourDoubles.at(i);
        
        // Check sum here to ensure it doesn't get opimized out
        Assert::IsFalse(sum==0.0, L"Exception should have been thrown");
      }
      catch (const std::out_of_range &)
      {
        // Good
      }      
    }

    TEST_METHOD(OutOfRangeMutableAt)
    {
      HeapArray<double, 4> fourDoubles;

      try
      {
        for (size_t i = 0; i < 5; ++i)
          fourDoubles.at(i) += 1.0;

        // Check sum here to ensure it doesn't get opimized out
        Assert::IsFalse(true, L"Exception should have been thrown");
      }
      catch (const std::out_of_range &)
      {
        // Good
      }
    }
  };
}