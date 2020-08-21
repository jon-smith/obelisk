#include "catch.hpp"

#include <map>

#include "obelisk/FormatString.hpp"
#include "obelisk/ImageFilters.hpp"
#include "obelisk/StringFuncs.h"

using namespace obelisk;

namespace
{
template <typename T> void testEmptyBlur(unsigned width, unsigned height)
{
	std::vector<T> const empty(width * height);
	auto emptyCopy = empty;
	boxBlur3x3(emptyCopy, width);
	REQUIRE(empty == emptyCopy);
}

template <typename T> T sum(const std::vector<T> &values)
{
	T sum = {};
	for (auto const &v : values)
		sum += v;
	return sum;
}
} // namespace

TEST_CASE("BoxBlur3x3Test")
{
	testEmptyBlur<double>(100, 200);
	testEmptyBlur<float>(100, 200);
	testEmptyBlur<int>(100, 200);
	testEmptyBlur<unsigned>(100, 200);
	testEmptyBlur<uint8_t>(100, 200);
}

TEST_CASE("GaussianKernel_TestNormalisation")
{
	constexpr auto sigma = 2.0;
	for (size_t i = 0; i < 100; ++i)
	{
		SECTION("Double")
		{
			const auto doubleKernelNorm = create2DGaussianKernel<double, true>(i, sigma);

			INFO(formatString("Sigma: %f, Width: %d\nGaussian filter (unnormalised):", sigma, i));
			std::string matrixValues;
			for (auto const &v : doubleKernelNorm)
				matrixValues += std::to_string(v) + " ";
			INFO(matrixValues.c_str());

			if (i == 0)
			{
				REQUIRE(0.0 == sum(doubleKernelNorm));
			}
			else
			{
				REQUIRE(1.0 == sum(doubleKernelNorm));
			}
		}
		SECTION("Float")
		{
			const auto floatKernelNorm = create2DGaussianKernel<float, true>(i, float(sigma));
			if (i == 0)
			{
				REQUIRE(0.0f == sum(floatKernelNorm));
			}
			else
			{
				REQUIRE(1.0f == sum(floatKernelNorm));
			}
		}
	}
}

TEST_CASE("GaussianKernel_TestAgainstThirdPartyKernelValues")
{
	// A range of expected values from http://dev.theomader.com/gaussian-kernel-calculator/
	std::map<std::pair<double, size_t>, std::vector<double>> const expectedNormalisedKernels = {
		{{0.5, 3}, {0.024879, 0.107973, 0.024879, 0.107973, 0.468592, 0.107973, 0.024879, 0.107973, 0.024879}},
		{{2.0, 3}, {0.102059, 0.115349, 0.102059, 0.115349, 0.130371, 0.115349, 0.102059, 0.115349, 0.102059}},
		{{2.0, 5}, {0.023528, 0.033969, 0.038393, 0.033969, 0.023528, 0.033969, 0.049045, 0.055432, 0.049045, 0.033969, 0.038393, 0.055432, 0.062651,
					0.055432, 0.038393, 0.033969, 0.049045, 0.055432, 0.049045, 0.033969, 0.023528, 0.033969, 0.038393, 0.033969, 0.023528}},
		{{4.0, 3}, {0.108808, 0.112244, 0.108808, 0.112244, 0.115788, 0.112244, 0.108808, 0.112244, 0.108808}},
		{{6.0, 7}, {0.017727, 0.018999, 0.019805, 0.020081, 0.019805, 0.018999, 0.017727, 0.018999, 0.020362, 0.021226, 0.021522, 0.021226, 0.020362,
					0.018999, 0.019805, 0.021226, 0.022127, 0.022436, 0.022127, 0.021226, 0.019805, 0.020081, 0.021522, 0.022436, 0.022748, 0.022436,
					0.021522, 0.020081, 0.019805, 0.021226, 0.022127, 0.022436, 0.022127, 0.021226, 0.019805, 0.018999, 0.020362, 0.021226, 0.021522,
					0.021226, 0.020362, 0.018999, 0.017727, 0.018999, 0.019805, 0.020081, 0.019805, 0.018999, 0.017727}}};

	constexpr auto expectedKernelValueEpsilon = 0.000001;

	for (const auto &e : expectedNormalisedKernels)
	{
		const auto doubleKernelNorm = create2DGaussianKernel<double, true>(e.first.second, e.first.first);
		INFO(formatString("Checking kernel size for sigma = %f, width = %d", e.first.first, e.first.second));
		REQUIRE(e.second.size() == doubleKernelNorm.size());

		for (size_t i = 0; i < e.second.size(); ++i)
		{
			INFO(formatString("Checking kernel value at index %d for sigma = %f, width = %d", i, e.first.first, e.first.second));

			REQUIRE(Approx(e.second[i]).margin(expectedKernelValueEpsilon) == doubleKernelNorm[i]);
		}
	}
}
