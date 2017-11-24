#include "stdafx.h"
#include "CppUnitTest.h"
#include "ContainerMaker.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(ContainerMakerTests)
	{
	public:
		TEST_METHOD(TestGenerateVector)
		{
			auto const vZeroToNine = generateVector<size_t>([](size_t idx)
			{
				return idx;
			}, 0, 10);
			Assert::IsTrue(vZeroToNine == std::vector<size_t>({ 0,1,2,3,4,5,6,7,8,9 }));

			auto const vTwoToNine = generateVector<size_t>([](size_t idx)
			{
				return idx;
			}, 2, 8);
			Assert::IsTrue(vTwoToNine == std::vector<size_t>({ 2,3,4,5,6,7,8,9 }));
		}
	};
}