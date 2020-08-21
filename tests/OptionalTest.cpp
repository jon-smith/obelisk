#include "catch.hpp"

#include "obelisk/Optional.hpp"

using namespace obelisk;

namespace
{
template <bool UseThreadSafeOptional, typename T> static void copyAndAssignTest()
{
	using OptionalT = std::conditional_t<UseThreadSafeOptional, Optional_ThreadSafe<T>, Optional<T>>;

	OptionalT const original(7);

	SECTION("Copy construct")
	{
		OptionalT const copy(original);
		REQUIRE(original.value() == copy.value());
	}
	SECTION("Assign construct")
	{
		OptionalT const copy2 = original;
		REQUIRE(original.value() == copy2.value());
	}

	SECTION("Assign")
	{
		OptionalT copy3;
		copy3 = original;
		REQUIRE(original.value() == copy3.value());
	}
}

template <bool UseThreadSafeOptional, typename T, typename U> static bool testDefaultInit(T &&value, U &&alternativeValue)
{
	static_assert(std::is_same<std::decay_t<T>, std::decay_t<U>>::value, "Supplied values should have same type");

	using V = std::decay_t<T>;
	const auto copyOfValue = V(std::ref(value));
	const auto copyOfAlternative = V(std::ref(alternativeValue));

	using OptionalT = std::conditional_t<UseThreadSafeOptional, Optional_ThreadSafe<V>, Optional<V>>;

	REQUIRE(copyOfValue != copyOfAlternative);

	// Default initialized
	auto const testOptional = OptionalT();

	REQUIRE(!testOptional.hasValue());

	// Ensure value() throws on default init
	REQUIRE_THROWS_AS([&testOptional]() { const auto v = testOptional.value(); }(), std::runtime_error);

	// Ensure valueOr works
	REQUIRE(testOptional.valueOr(copyOfValue) == copyOfValue);

	return true;
}

template <bool UseThreadSafeOptional, typename T, typename U> static bool testValueInit(T &&value, U &&alternativeValue)
{
	static_assert(std::is_same<std::decay_t<T>, std::decay_t<U>>::value, "Supplied values should have same type");

	using V = std::decay_t<T>;
	auto const copyOfValue = V(std::ref(value));
	auto const copyOfAlternative = V(std::ref(alternativeValue));

	REQUIRE(copyOfValue != copyOfAlternative);

	using OptionalT = std::conditional_t<UseThreadSafeOptional, Optional_ThreadSafe<V>, Optional<V>>;

	// Value initialized
	auto testOptional = OptionalT(value);

	REQUIRE(testOptional.hasValue());
	REQUIRE(testOptional.value() == copyOfValue);
	REQUIRE(testOptional.valueOr(copyOfAlternative) == copyOfValue);

	testOptional.reset();

	REQUIRE(!testOptional.hasValue());

	testOptional.set(alternativeValue);
	REQUIRE(testOptional.value() == alternativeValue);

	return true;
}

template <bool UseThreadSafeOptional, typename T, typename U> static bool test(T value, U alternativeValue)
{
	return testDefaultInit<UseThreadSafeOptional>(value, alternativeValue) && testValueInit<UseThreadSafeOptional>(value, alternativeValue);
}

} // namespace

TEST_CASE("OptionalDouble")
{
	REQUIRE(test<true>(1.0, 2.0));
	REQUIRE(test<false>(1.0, 2.0));
}

TEST_CASE("OptionalInt")
{
	REQUIRE(test<true>(1, 2));
	REQUIRE(test<false>(1, 2));
}

TEST_CASE("OptionalString")
{
	REQUIRE(test<true>(std::string(), std::string("chump")));
	REQUIRE(test<false>(std::string(), std::string("chump")));
}

TEST_CASE("OptionalCopyAndAssign")
{
	copyAndAssignTest<true, int>();
	copyAndAssignTest<false, int>();
}
