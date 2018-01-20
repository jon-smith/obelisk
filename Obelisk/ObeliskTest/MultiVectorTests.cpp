#include "stdafx.h"
#include "CppUnitTest.h"

#include "StringFuncs.h"
#include "MultiVector.hpp"

#include <string>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(MultiVectorTest)
	{
	public:

		// Just test that they construct
		TEST_METHOD(DefaultConstruct)
		{
			const auto threeVecsDefault = MultiVector<double, int, std::string>();
			Assert::AreEqual(size_t(0), threeVecsDefault.size());
			Assert::IsTrue(threeVecsDefault.isEmpty());
		}

		TEST_METHOD(CopyConstruct)
		{
			std::vector<double> dV(5);
			std::vector<int> iV(5);
			std::vector<std::string> strV(5);

			const auto threeVecsCopied = MakeMultiVector(dV, iV, strV);
			const auto threeVecsCopied2 = MultiVector<double, int, std::string>(dV, iV, strV);

			// Test contents identical
			Assert::AreEqual(size_t(5), threeVecsCopied.size());
			Assert::AreEqual(size_t(5), threeVecsCopied2.size());
			Assert::IsFalse(threeVecsCopied.isEmpty());
			Assert::IsFalse(threeVecsCopied2.isEmpty());
		}

		TEST_METHOD(RefConstruct)
		{
			std::vector<double> dV(5);
			std::vector<int> iV(5);
			std::vector<std::string> strV(5);

			const auto threeVecRefs = MakeMultiVectorRef(dV, iV, strV);
			const auto threeVecRefs2 = MultiVectorRef<double, int, std::string>(dV, iV, strV);

			// Test contents identical
			Assert::AreEqual(size_t(5), threeVecRefs.size());
			Assert::AreEqual(size_t(5), threeVecRefs2.size());
			Assert::IsFalse(threeVecRefs.isEmpty());
			Assert::IsFalse(threeVecRefs2.isEmpty());
		}

		TEST_METHOD(CRefConstruct)
		{
			const std::vector<double> dV(5);
			const std::vector<int> iV(5);
			const std::vector<std::string> strV(5);

			const auto threeVecRefs = MakeMultiVectorRef(dV, iV, strV);
			static_assert(
				std::is_same_v<std::decay_t<decltype(threeVecRefs)>,
				MultiVectorCRef<double, int, std::string>>, "Make should construct CRef version");

			const auto threeVecRefs2 = MultiVectorCRef<double, int, std::string>(dV, iV, strV);

			// Test contents identical
			Assert::AreEqual(size_t(5), threeVecRefs.size());
			Assert::AreEqual(size_t(5), threeVecRefs2.size());
			Assert::IsFalse(threeVecRefs.isEmpty());
			Assert::IsFalse(threeVecRefs2.isEmpty());
		}

		TEST_METHOD(SizeConstruct)
		{
			const auto threeVecsSize3 = MultiVector<double, int, std::string>(3);

			// Test size 3 with all default init
			Assert::AreEqual(size_t(3), threeVecsSize3.size());
			Assert::IsFalse(threeVecsSize3.isEmpty());
		}

		TEST_METHOD(ThrowOnInconsistentSizes)
		{
			std::vector<double> dV(5);
			std::vector<int> dI(4);

			Assert::ExpectException<std::exception>([&]() {MakeMultiVector(dV, dI); }, L"");
			Assert::ExpectException<std::exception>([&]() {MakeMultiVectorRef(dV, dI); }, L"");
			Assert::ExpectException<std::exception>([&]() {MultiVector<double, int>(dV, dI); }, L"");
			Assert::ExpectException<std::exception>([&]() {MultiVectorRef<double, int>(dV, dI); }, L"");
		}
	};
}