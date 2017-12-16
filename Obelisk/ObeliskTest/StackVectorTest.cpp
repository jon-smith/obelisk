#include "stdafx.h"
#include "CppUnitTest.h"
#include "StackVector.hpp"
#include "StringFuncs.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(StackVectorTest)
	{
	public:

		TEST_METHOD(DefaultInit)
		{
			StackVector<double, 10> vec;
			Assert::AreEqual(vec.size(), size_t{}, L"Should be empty");
		}

		TEST_METHOD(ListInitConstructFull)
		{
			StackVector<double, 5> fiveDoubles{ 1.0,1.0,1.0,1.0,1.0 };
			size_t count = 0;
			for (auto const &v : fiveDoubles)
			{
				Assert::AreEqual(v, 1.0, L"All values should be 1.0");
				++count;
			}
			Assert::AreEqual(count, size_t{ 5 }, L"should iterate through 5 elements");
		}

		TEST_METHOD(ListInitConstructHalf)
		{
			StackVector<double, 4> fourDoubles{ 1.0,1.0 };
			for (size_t i = 0; i<fourDoubles.size(); ++i)
				if (i < 2)
					Assert::AreEqual(fourDoubles[i], 1.0, L"First two values should be 1.0");
				else
					Assert::AreEqual(fourDoubles[i], double{}, L"Other values should be default initialized");
		}

		TEST_METHOD(Copy)
		{
			StackVector<double, 2> twoDoubles{ 1.0,1.0 };
			auto const anotherTwoDoubles = twoDoubles;

			for (size_t i = 0; i<twoDoubles.size(); ++i)
				Assert::AreEqual(twoDoubles[i], anotherTwoDoubles[i], L"Contents should be identical");
		}

		TEST_METHOD(Fill)
		{
			StackVector<double, 5> doubles(5);

			doubles.fill(1.0);

			size_t count = 0;
			for (auto const &v : doubles)
			{
				Assert::AreEqual(v, 1.0, L"All values should be 1.0");
				++count;
			}
			Assert::AreEqual(count, size_t{ 5 }, L"should iterate through 5 elements");
		}

		TEST_METHOD(PushBack)
		{
			StackVector<double, 10> vec;

			for (size_t i = 0; i < 10; ++i)
				vec.push_back(static_cast<double>(i));
			Assert::AreEqual(vec.size(), size_t{ 10 }, L"should have 10 elements");

			size_t count = 0;
			for (auto const &v : vec)
			{
				Assert::AreEqual(v, static_cast<double>(count), L"");
				++count;
			}

			Assert::AreEqual(count, size_t{ 10 }, L"should iterate through 10 elements");
		}

		TEST_METHOD(EmplaceBack)
		{
			StackVector<double, 10> vec;

			for (size_t i = 0; i < 10; ++i)
				vec.emplace_back(static_cast<double>(i));
			Assert::AreEqual(vec.size(), size_t{ 10 }, L"should have 10 elements");

			size_t count = 0;
			for (auto const &v : vec)
			{
				Assert::AreEqual(v, static_cast<double>(count), L"");
				++count;
			}
			Assert::AreEqual(count, size_t{ 10 }, L"should iterate through 10 elements");
		}

		TEST_METHOD(EmplaceBackUnchecked)
		{
			StackVector<double, 10> vec;

			for (size_t i = 0; i < 10; ++i)
				vec.emplace_back_unchecked(static_cast<double>(i));
			Assert::AreEqual(vec.size(), size_t{ 10 }, L"should have 10 elements");

			size_t count = 0;
			for (auto const &v : vec)
			{
				Assert::AreEqual(v, static_cast<double>(count), L"");
				++count;
			}
			Assert::AreEqual(count, size_t{ 10 }, L"should iterate through 10 elements");
		}

		TEST_METHOD(OutOfRangeConstAt)
		{
			StackVector<double, 4> fourDoubles(4);

			try
			{
				double sum = 0;
				for (size_t i = 0; i < 5; ++i)
					sum += fourDoubles.at(i);

				// Check sum here to ensure it doesn't get opimized out
				Assert::IsFalse(sum == 0.0, L"Exception should have been thrown");
			}
			catch (const std::out_of_range &)
			{
				// Good
			}
		}

		TEST_METHOD(OutOfRangeMutableAt)
		{
			StackVector<double, 4> fourDoubles(4);

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