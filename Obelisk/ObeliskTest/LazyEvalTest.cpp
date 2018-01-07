#include "stdafx.h"
#include "CppUnitTest.h"
#include "LazyEval.hpp"

#include <future>
#include <vector>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(LazyEvalTest)
	{
	public:
		TEST_METHOD(TestLazyEvaluation)
		{
			constexpr int result = 55;

			unsigned evalCount = 0;
			auto const evalFn =[&evalCount, result](){
				evalCount++;
				return result;
			};

			// Construct
			LazyEval<int> lazyCalc(evalFn);
			// Check the function hasn't yet been called
			Assert::IsTrue(evalCount == 0, L"EvalFn called early");

			// Multiple check to make sure the result doesnt change and the eval func only gets called once
			for (int i = 0; i < 5; ++i)
			{
				// Confirm that we get the right result
				const auto lazyEvalResult = lazyCalc.get();
				Assert::IsTrue(result == lazyEvalResult);
				// Confirm eval func is only called once
				Assert::IsTrue(evalCount == 1);
			}
		}

		TEST_METHOD(TestLazyEvaluation_MultiThread)
		{
			// Create a struct with a few members
			// to make it more likely to fail if there's a thread safety problem
			struct StructWithAFewVariables
			{
				StructWithAFewVariables(double d, int i, std::string str) : d(d), i(i), str(str) {};
				double d;
				int i;
				std::string str;
			};
			const StructWithAFewVariables result(5.5, 1, "banana");

			unsigned evalCount = 0;
			auto const evalFn = [&evalCount, result]() {
				evalCount++;
				return result;
			};

			// Construct
			LazyEvalThreadSafe<StructWithAFewVariables> lazyCalc(evalFn);
			// Check the function hasn't yet been called
			Assert::IsTrue(evalCount == 0, L"EvalFn called early");

			// Spawn loads of threads to check safety
			// Make sure the result doesnt change
			// and the eval func only gets called once
			// This isn't a perfect test of thread safety - but at least it tries...

			std::vector<std::future<void>> threads;

			for (int i = 0; i < 256; ++i)
			{
				threads.push_back(std::async(std::launch::async, [&lazyCalc, &evalCount, &result](){
					// Confirm that we get the right result
					const auto lazyEvalResult = lazyCalc.get();
					Assert::IsTrue(result.d == lazyEvalResult.d);
					Assert::IsTrue(result.i == lazyEvalResult.i);
					Assert::IsTrue(result.str == lazyEvalResult.str);
					// Confirm eval func is only called once
					Assert::IsTrue(evalCount == 1);
				}));
			}

			for (const auto &t : threads)
				t.wait();
		}
	};
}