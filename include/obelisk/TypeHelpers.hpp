#pragma once

namespace obelisk
{
	// Get element type of a container, retaining const-ness
	template <typename T>
	using ElementType = std::remove_reference_t<decltype(*std::begin(std::declval<T&>()))>;

	// Get element type of a container, retaining const-ness
	template <typename T>
	using IteratorType = decltype(std::begin(std::declval<T&>()));
}