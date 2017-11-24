#include "stdafx.h"
#include "CppUnitTest.h"
#include "Optional.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;
using namespace obelisk;

namespace ObeliskTest
{
	TEST_CLASS(OptionalTest)
	{
	public:

		TEST_METHOD(Double)
		{
			Assert::IsTrue(test<true>(1.0, 2.0), L"Failed double");
			Assert::IsTrue(test<false>(1.0, 2.0), L"Failed double");
		}

		TEST_METHOD(Int)
		{
			Assert::IsTrue(test<true>(1, 2), L"Failed int");
			Assert::IsTrue(test<false>(1, 2), L"Failed int");
		}

		TEST_METHOD(String)
		{
			Assert::IsTrue(test<true>(std::string(), std::string("chump")), L"Failed string");
			Assert::IsTrue(test<false>(std::string(), std::string("chump")), L"Failed string");
		}

		TEST_METHOD(CopyAndAssign)
		{
			copyAndAssignTest<true, int>();
			copyAndAssignTest<false, int>();
		}

	private:

		template <bool UseThreadSafeOptional, typename T>
		static void copyAndAssignTest()
		{
			using OptionalT = std::conditional_t<UseThreadSafeOptional, Optional_ThreadSafe<T>, Optional_ThreadSafe<T>>;

			OptionalT const original(7);

			OptionalT const copy(original);
			Assert::AreEqual(original.value(), copy.value(), L"Failed copy construct");

			OptionalT const copy2 = original;
			Assert::AreEqual(original.value(), copy2.value(), L"Failed assign construct");

			OptionalT copy3;
			copy3 = original;
			Assert::AreEqual(original.value(), copy3.value(), L"Failed assign");
		}

		template <bool UseThreadSafeOptional, typename T, typename U>
		static bool test(T value, U alternativeValue)
		{
			return
				testDefaultInit<UseThreadSafeOptional>(value, alternativeValue) &&
				testValueInit<UseThreadSafeOptional>(value, alternativeValue);
		}

		template <bool UseThreadSafeOptional, typename T, typename U>
		static bool testDefaultInit(T&& value, U&& alternativeValue)
		{
			static_assert(std::is_same<std::decay_t<T>, std::decay_t<U>>::value, "Supplied values should have same type");
			
			using V = std::decay_t<T>;
			auto const copyOfValue = V(std::ref(value));
			auto const copyOfAlternative = V(std::ref(alternativeValue));

			using OptionalT = std::conditional_t<UseThreadSafeOptional, Optional_ThreadSafe<V>, Optional_ThreadSafe<V>>;

			Assert::AreNotSame(copyOfValue, copyOfAlternative, L"Supplied values should be the same");

			// Default initialized
			auto const testOptional = OptionalT();

			Assert::IsFalse(testOptional.hasValue(), L"Default init should not have value");

			// Ensure value() throws on default init
			try
			{
				auto const v = testOptional.value();
				Assert::IsFalse(true, L"value() should throw");
			}
			catch (const std::exception &)
			{
				// Good
			}

			// Ensure valueOr works
			Assert::AreEqual(testOptional.valueOr(copyOfValue), copyOfValue, L"valueOr should return the 'or'");

			return true;
		}

		template <bool UseThreadSafeOptional, typename T, typename U>
		static bool testValueInit(T&& value, U&& alternativeValue)
		{
			static_assert(std::is_same<std::decay_t<T>, std::decay_t<U>>::value, "Supplied values should have same type");

			using V = std::decay_t<T>;
			auto const copyOfValue = V(std::ref(value));
			auto const copyOfAlternative = V(std::ref(alternativeValue));

			Assert::AreNotSame(copyOfValue, copyOfAlternative, L"Supplied values should be the same");

			using OptionalT = std::conditional_t<UseThreadSafeOptional, Optional_ThreadSafe<V>, Optional_ThreadSafe<V>>;

			// Value initialized
			auto testOptional = OptionalT(value);

			Assert::IsTrue(testOptional.hasValue(), L"Value init should have value");
			Assert::AreEqual(testOptional.value(), copyOfValue);
			Assert::AreEqual(testOptional.valueOr(copyOfAlternative), copyOfValue);

			testOptional.reset();

			Assert::IsFalse(testOptional.hasValue(), L"Value has been reset, should not contain anything");

			testOptional.set(alternativeValue);
			Assert::AreEqual(testOptional.value(), alternativeValue);

			return true;
		}

	};
}