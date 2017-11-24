#include "stdafx.h"
#include "CppUnitTest.h"

#include <map>

#include "ImageFilters.hpp"
#include "StringFuncs.h"
#include "FormatString.hpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(ImageFilterTests)
	{
		template <typename T>
		void testEmptyBlur(unsigned width, unsigned height)
		{
			std::vector<T> const empty(width*height);
			auto emptyCopy = empty;
			boxBlur3x3(emptyCopy, width);
			Assert::IsTrue(empty == emptyCopy, L"Blurred empty is not empty");
		}

		template <typename T>
		T sum(const std::vector<T> &values)
		{
			T sum = {};
			for (auto const &v : values)
				sum += v;
			return sum;
		}

	public:

		TEST_METHOD(BoxBlur3x3Test)
		{
			testEmptyBlur<double>(100, 200);
			testEmptyBlur<float>(100, 200);
			testEmptyBlur<int>(100, 200);
			testEmptyBlur<unsigned>(100, 200);
			testEmptyBlur<uint8_t>(100, 200);
		}

		TEST_METHOD(GaussianKernel_TestNormalisation)
		{
			auto constexpr sigma = 2.0;
			for (size_t i = 0; i < 100; ++i)
			{
				auto const floatKernelNorm = create2DGaussianKernel<float, true>(i, float(sigma));
				if (i == 0)
					Assert::AreEqual(0.0f, sum(floatKernelNorm), i*std::numeric_limits<float>::epsilon(), L"Sum of 0 kernel should equal 0");
				else
					Assert::AreEqual(1.0f, sum(floatKernelNorm), i*std::numeric_limits<float>::epsilon(), L"Sum of normalised should equal 1");

				auto const doubleKernelNorm = create2DGaussianKernel<double, true>(i, sigma);
				if (i == 0)
					Assert::AreEqual(0.0, sum(doubleKernelNorm), i*std::numeric_limits<double>::epsilon(), L"Sum of 0 kernel should equal 0");
				else
					Assert::AreEqual(1.0, sum(doubleKernelNorm), i*std::numeric_limits<double>::epsilon(), L"Sum of normalised should equal 1");

				constexpr auto printValues = false;
				if (printValues)
				{
					Logger::WriteMessage(formatString("Sigma: %f, Width: %d\nGaussian filter (unnormalised):", sigma, i).c_str());
					std::string matrixValues;
					for (auto const &v : doubleKernelNorm)
						matrixValues += std::to_string(v) + " ";
					Logger::WriteMessage(matrixValues.c_str());
				}
			}	
		}

		TEST_METHOD(GaussianKernel_TestAgainstThirdPartyKernelValues)
		{
			// A range of expected values from http://dev.theomader.com/gaussian-kernel-calculator/
			std::map<std::pair<double, size_t>, std::vector<double>> const expectedNormalisedKernels =
			{
				{ { 2.0, 3 },{
					0.102059 , 0.115349 , 0.102059,
					0.115349, 0.130371, 0.115349,
					0.102059, 0.115349, 0.102059
				} },
				{ { 2.0, 5 },{
					0.023528,	0.033969,	0.038393,	0.033969,	0.023528,
					0.033969,	0.049045,	0.055432,	0.049045,	0.033969,
					0.038393,	0.055432,	0.062651,	0.055432,	0.038393,
					0.033969,	0.049045,	0.055432,	0.049045,	0.033969,
					0.023528,	0.033969,	0.038393,	0.033969,	0.023528
				} },
				{ { 4.0, 3 },{
					0.108808 , 0.112244 , 0.108808,
					0.112244, 0.115788, 0.112244,
					0.108808, 0.112244, 0.108808
				} },
				{ { 6.0, 7 },{
					0.017727,	0.018999,	0.019805,	0.020081,	0.019805,	0.018999,	0.017727,
					0.018999,	0.020362,	0.021226,	0.021522,	0.021226,	0.020362,	0.018999,
					0.019805,	0.021226,	0.022127,	0.022436,	0.022127,	0.021226,	0.019805,
					0.020081,	0.021522,	0.022436,	0.022748,	0.022436,	0.021522,	0.020081,
					0.019805,	0.021226,	0.022127,	0.022436,	0.022127,	0.021226,	0.019805,
					0.018999,	0.020362,	0.021226,	0.021522,	0.021226,	0.020362,	0.018999,
					0.017727,	0.018999,	0.019805,	0.020081,	0.019805,	0.018999,	0.017727
				} }
			};

			constexpr auto expectedKernelValueEpsilon = 0.000001;

			for (auto const &e : expectedNormalisedKernels)
			{
				auto const doubleKernelNorm = create2DGaussianKernel<double, true>(e.first.second, e.first.first);
				Assert::AreEqual(e.second.size(), doubleKernelNorm.size(),
					formatString(L"Kernel size incorrect for sigma = %f, width = %d", e.first.first, e.first.second).c_str());

				for (size_t i = 0; i<e.second.size(); ++i)
					Assert::AreEqual(e.second[i], doubleKernelNorm[i], expectedKernelValueEpsilon,
						formatString(L"Incorrect kernel value at index %d for sigma = %f, width = %d", i, e.first.first, e.first.second).c_str());
			}
		}

		TEST_METHOD(GaussianKernel_TestAgainstThirdPartyWeights)
		{
			// A range of expected weights outside the kernel from http://dev.theomader.com/gaussian-kernel-calculator/
			std::map<std::pair<double, size_t>, double> const expectedUnnormalisedWeightsOutsideKernel =
			{
				{ { 0.5, 3 }, 0.0027 },
				{ { 1.0, 3 }, 0.1336 },
				{ { 2.0, 3 }, 0.4533 },
				{ { 4.0, 3 }, 0.7077 },
				{ { 8.0, 3 }, 0.8513 },
				{ { 16.0, 3 }, 0.9253 },
				{ { 0.5, 7 }, 0.0000 },
				{ { 1.0, 7 }, 0.0005 },
				{ { 2.0, 7 }, 0.0801 },
				{ { 4.0, 7 }, 0.3816 },
				{ { 0.5, 11 }, 0.0000 },
				{ { 1.0, 11 }, 0.0000 },
				{ { 2.0, 11 }, 0.0060 },
				{ { 4.0, 11 }, 0.1691 },
			};

			constexpr auto expectedWeightValueEpsilon = 0.0001;

			for (auto const &e : expectedUnnormalisedWeightsOutsideKernel)
			{
				auto const kernel = create2DGaussianKernel<double, false>(e.first.second, e.first.first);
				Assert::AreEqual(e.second, 1.0 - sum(kernel), expectedWeightValueEpsilon,
					formatString(L"Kernel weight incorrect for sigma = %f, width = %d", e.first.first, e.first.second).c_str());
			}
		}
	};
}