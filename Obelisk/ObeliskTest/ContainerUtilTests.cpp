#include "stdafx.h"
#include "CppUnitTest.h"
#include "ContainerUtils.h"
#include "StringFuncs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
  TEST_CLASS(ContainerUtilTests)
  {
  public:

    TEST_METHOD(RestrictIncreasingRange)
    {
      std::vector<int> const increasing = { 1,2,3,4,5 };
      Assert::IsTrue(increasing == restrictToIncreasingRange(2, increasing), L"Increasing vector should not be reduced");
      
      std::vector<int> const decreasing = { 5,4,3,2,1 };
      Assert::IsTrue(std::vector<int>(1,decreasing[2]) == restrictToIncreasingRange(2, decreasing), L"Decreasing vector should be reduced to single item");

      std::vector<int> const increasingInMiddle = { 5,4,3,2,1,2,3,4,5,4,3,4,5 };
      Assert::IsTrue(std::vector<int>({1,2,3,4,5}) == restrictToIncreasingRange(4, increasingInMiddle), L"Vector is not restricted");

      std::vector<int> flat = { 1,1,1 };
      Assert::IsTrue(std::vector<int>(1, flat[1]) == restrictToIncreasingRange(1, flat), L"Flat should be restricted to single item");
    }

    TEST_METHOD(RestrictDecreasingRange)
    {
      std::vector<int> const increasing = { 1,2,3,4,5 };
      Assert::IsTrue(std::vector<int>(1, increasing[2]) == restrictToDecreasingRange(2, increasing), L"Increasing vector should be reduced to a single item");

      std::vector<int> const decreasing = { 5,4,3,2,1 };
      Assert::IsTrue(decreasing == restrictToDecreasingRange(2, decreasing), L"Decreasing vector should not be reduced");

      std::vector<int> const decreasingInMiddle = { 5,4,3,2,1,2,3,4,5,4,3,4,5 };
      Assert::IsTrue(std::vector<int>({ 5,4,3,2,1 }) == restrictToDecreasingRange(4, decreasingInMiddle), L"Vector is not restricted");

      std::vector<int> flat = { 1,1,1 };
      Assert::IsTrue(std::vector<int>(1, flat[1]) == restrictToDecreasingRange(1, flat), L"Flat should be restricted to single item");     
    }

    TEST_METHOD(RestrictIncreasingRangeCustomComparator)
    {
      std::vector<std::pair<int, int>> const decreasingOnFirstIncreasingOnSecond =
      { {5,1},{4,2}, {3,3}, {2,4}, {1,5} };

      // Compare on second (should all increase)
      Assert::IsTrue(decreasingOnFirstIncreasingOnSecond == restrictToIncreasingRange(2, decreasingOnFirstIncreasingOnSecond,
        [](std::pair<int, int> l, std::pair<int, int> r) {return l.second < r.second; }), L"Increasing vector should not be reduced");

      // Compare on first - should only keep from item
      Assert::IsTrue(std::vector<std::pair<int, int>>(1, decreasingOnFirstIncreasingOnSecond[2]) == restrictToIncreasingRange(2, decreasingOnFirstIncreasingOnSecond,
        [](std::pair<int, int> l, std::pair<int, int> r) {return l.first < r.first; }), L"Decreasing vector should be reduced to a single item");
    }

    TEST_METHOD(RestrictRangeThrows)
    {
      // Increasing out of range
      bool exceptionCaught = false;
      std::vector<int> vec = { 1,1,1,1 };
      try
      {
        restrictToIncreasingRange(vec.size(), vec);
      }
      catch (const std::invalid_argument &)
      {
        exceptionCaught = true;
      }

      Assert::IsTrue(exceptionCaught, L"Exception should have been thrown");

      // Increasing empty
      exceptionCaught = false;
      try
      {
        restrictToIncreasingRange(0, std::vector<int>());
      }
      catch (const std::invalid_argument &)
      {
        exceptionCaught = true;
      }

      Assert::IsTrue(exceptionCaught, L"Exception should have been thrown");

      // Decreasing out of range
      exceptionCaught = false;
      try
      {
        restrictToDecreasingRange(vec.size(), vec);
      }
      catch (const std::invalid_argument &)
      {
        exceptionCaught = true;
      }

      Assert::IsTrue(exceptionCaught, L"Exception should have been thrown");

      // Decreasing empty
      exceptionCaught = false;
      try
      {
        restrictToDecreasingRange(0, std::vector<int>());
      }
      catch (const std::invalid_argument &)
      {
        exceptionCaught = true;
      }

      Assert::IsTrue(exceptionCaught, L"Exception should have been thrown");

    }
  };
}