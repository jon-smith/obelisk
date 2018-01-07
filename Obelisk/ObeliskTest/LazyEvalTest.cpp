#include "stdafx.h"
#include "CppUnitTest.h"
#include "LazyEval.hpp"

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
	};
}