#include "stdafx.h"
#include "CppUnitTest.h"

#include "StringFuncs.h"
#include "MultiVector.hpp"

#include <string>
#include <memory>

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

			// Sizes
			Assert::AreEqual(size_t(5), threeVecsCopied.size(), L"Make copy wrong size");
			Assert::AreEqual(size_t(5), threeVecsCopied2.size(), L"Construct copy wrong size");
			// Empty
			Assert::IsFalse(threeVecsCopied.isEmpty(), L"Make copy not empty");
			Assert::IsFalse(threeVecsCopied2.isEmpty(), L"Make copy not empty");
			// Contents
			Assert::IsTrue(dV == threeVecsCopied.get<0>(), L"");
			Assert::IsTrue(iV == threeVecsCopied.get<1>(), L"");
			Assert::IsTrue(strV == threeVecsCopied.get<2>(), L"");
			Assert::IsTrue(dV == threeVecsCopied2.get<0>(), L"");
			Assert::IsTrue(iV == threeVecsCopied2.get<1>(), L"");
			Assert::IsTrue(strV == threeVecsCopied2.get<2>(), L"");
		}

		TEST_METHOD(MoveConstruct)
		{
			// Create multivectors with a move-only type
			const auto twoVecs = MakeMultiVector(std::vector<int>(2), std::vector<std::unique_ptr<int>>(2));
			const auto twoVecs2 = MultiVector<int, std::unique_ptr<int>>(std::vector<int>(2), std::vector<std::unique_ptr<int>>(2));

			// Sizes
			Assert::AreEqual(size_t(2), twoVecs.size());
			Assert::AreEqual(size_t(2), twoVecs2.size());
			// Empty
			Assert::IsFalse(twoVecs.isEmpty());
			Assert::IsFalse(twoVecs2.isEmpty());
			// Contents
			Assert::IsTrue(std::vector<int>(2) == twoVecs.get<0>(), L"");
			Assert::IsTrue(std::vector<std::unique_ptr<int>>(2) == twoVecs.get<1>(), L"");
			Assert::IsTrue(std::vector<int>(2) == twoVecs2.get<0>(), L"");
			Assert::IsTrue(std::vector<std::unique_ptr<int>>(2) == twoVecs2.get<1>(), L"");
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
			// Contents
			Assert::IsTrue(dV == threeVecRefs.get<0>(), L"");
			Assert::IsTrue(iV == threeVecRefs.get<1>(), L"");
			Assert::IsTrue(strV == threeVecRefs.get<2>(), L"");
			Assert::IsTrue(dV == threeVecRefs2.get<0>(), L"");
			Assert::IsTrue(iV == threeVecRefs2.get<1>(), L"");
			Assert::IsTrue(strV == threeVecRefs2.get<2>(), L"");
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
			// Contents
			Assert::IsTrue(dV == threeVecRefs.get<0>(), L"");
			Assert::IsTrue(iV == threeVecRefs.get<1>(), L"");
			Assert::IsTrue(strV == threeVecRefs.get<2>(), L"");
			Assert::IsTrue(dV == threeVecRefs2.get<0>(), L"");
			Assert::IsTrue(iV == threeVecRefs2.get<1>(), L"");
			Assert::IsTrue(strV == threeVecRefs2.get<2>(), L"");
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

			Assert::ExpectException<std::invalid_argument>([&]() {MakeMultiVector(dV, dI); }, L"");
			Assert::ExpectException<std::invalid_argument>([&]() {MakeMultiVectorRef(dV, dI); }, L"");
			Assert::ExpectException<std::invalid_argument>([&]() {MakeMultiVectorCRef(dV, dI); }, L"");
			Assert::ExpectException<std::invalid_argument>([&]() {MultiVector<double, int>(dV, dI); }, L"");
			Assert::ExpectException<std::invalid_argument>([&]() {MultiVectorRef<double, int>(dV, dI); }, L"");
			Assert::ExpectException<std::invalid_argument>([&]() {MultiVectorCRef<double, int>(dV, dI); }, L"");
		}
	};
}