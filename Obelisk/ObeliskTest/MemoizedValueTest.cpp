#include "stdafx.h"
#include "CppUnitTest.h"
#include "MemoizedValue.h"
#include "StringFuncs.h"
#include "FormatString.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(MemoizedValueTest)
	{
	public:

		TEST_METHOD(Double)
		{
			auto const lambdaCalc = []()
			{
				auto value = 0.0;
				for (auto d = 0.0; d < 1.0; d += 0.000001)
				{
					value += exp(d);
				}
				return value;
			};

			auto const cmpValue = lambdaCalc();

			// 10x test of speed
			for (size_t i = 0; i < 10; ++i)
			{
				Logger::WriteMessage(formatString("Testing memoization speedup, iteration %d", i).c_str());

				MemoizedValue<double> m(lambdaCalc);

				auto const beforeFirst = std::chrono::steady_clock::now();
				auto const first = m.get();
				auto const timeFirst = std::chrono::steady_clock::now() - beforeFirst;

				auto const beforeSecond = std::chrono::steady_clock::now();
				auto const second = m.get();
				auto const timeSecond = std::chrono::steady_clock::now() - beforeSecond;

				Logger::WriteMessage(formatString("Time first: %d, second: %d", timeFirst.count(), timeSecond.count()).c_str());
				Assert::IsTrue(timeSecond.count() * 100 < timeFirst.count(), L"Second get() should significantly faster than first");
				Assert::AreEqual(first, second, L"get() should return the same");
				Assert::AreEqual(first, cmpValue, L"values should always be equal to expected result");

				m.reset();

				auto const postResetBeforeFirst = std::chrono::steady_clock::now();
				auto const postResetFirst = m.get();
				auto const postResetTimeFirst = std::chrono::steady_clock::now() - postResetBeforeFirst;

				auto const postResetBeforeSecond = std::chrono::steady_clock::now();
				auto const postResetSecond = m.get();
				auto const postResetTimeSecond = std::chrono::steady_clock::now() - postResetBeforeSecond;

				Logger::WriteMessage(formatString("Post reset time first: %d, second: %d", postResetTimeFirst.count(), postResetTimeSecond.count()).c_str());
				Assert::IsTrue(postResetTimeSecond.count() * 100 < postResetTimeFirst.count(), L"(Post reset) Second get() should significantly faster than first");
				Assert::AreEqual(postResetFirst, postResetSecond, L"get() should return the same"); // This is to ensure compiler doesn't optimize out m.get() calls

			}
		}
	};
}