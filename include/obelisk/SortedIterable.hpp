#pragma once

#include <functional>
#include <vector>

#include "obelisk/TypeHelpers.hpp"

namespace obelisk
{

template <typename T>
struct SortedIterator
{
  private:
	using UnderlyingIt = IteratorType<std::vector<T *>>;
	UnderlyingIt it = nullptr;
	struct Holder
	{
		UnderlyingIt it = nullptr;
		T &operator*() { return **it; }
	};

  public:
	SortedIterator(UnderlyingIt it)
		: it(it){

		  };

	T &operator*() const { return **it; }

	bool operator==(const SortedIterator &other) const { return other.it == it; }

	bool operator!=(const SortedIterator &other) const { return !(*this == other); }

	Holder operator++(int)
	{
		Holder ret{it};
		++*this;
		return ret;
	}

	SortedIterator &operator++()
	{
		++it;
		return *this;
	}
};

namespace impl
{
template <typename ResultT, typename InputT>
using GetValFunc = std::function<ResultT(const InputT &)>;

template <typename T>
using CompFunc = std::function<bool(const T &, const T &)>;

template <typename InputT, typename GetVal>
CompFunc<InputT> getValToLessThan(GetVal &&fn)
{
	return [fn](const InputT &a, const InputT &b) { return fn(a) < fn(b); };
}

} // namespace impl

template <typename V>
struct SortedIterable
{
	using It = SortedIterator<V>;
	using CompFunc = impl::CompFunc<V>;

	template <typename ContainerT>
	SortedIterable(const ContainerT &c, CompFunc lessThan)
	{
		for (auto &v : c)
		{
			sorted.push_back(&v);
		}
		sortHelper(lessThan);
	}

	template <typename ContainerT>
	SortedIterable(ContainerT &c, CompFunc lessThan)
	{
		for (auto &v : c)
		{
			sorted.push_back(&v);
		}
		sortHelper(lessThan);
	}

	It begin() { return It(std::begin(sorted)); }
	It end() { return It(std::end(sorted)); }

  private:
	void sortHelper(CompFunc lessThan)
	{
		std::sort(std::begin(sorted), std::end(sorted), [lessThan](V *a, V *b) { return lessThan(*a, *b); });
	}

	std::vector<V *> sorted;
};

template <typename ContainerT, typename CompFunc, typename V = ElementType<ContainerT>>
SortedIterable<const V> makeSorted(const ContainerT &c, CompFunc &&lessThan)
{
	return SortedIterable<const V>(c, lessThan);
}

template <typename ContainerT, typename CompFunc, typename V = ElementType<ContainerT>>
SortedIterable<V> makeSorted(ContainerT &c, CompFunc &&lessThan)
{
	return SortedIterable<V>(c, lessThan);
}

template <typename ContainerT, typename GetValFunc, typename V = ElementType<ContainerT>>
SortedIterable<const V> makeSortedOn(const ContainerT &c, GetValFunc &&sortOn)
{
	return SortedIterable<const V>(c, impl::getValToLessThan<V>(sortOn));
}

template <typename ContainerT, typename GetValFunc, typename V = ElementType<ContainerT>>
SortedIterable<V> makeSortedOn(ContainerT &c, GetValFunc &&sortOn)
{
	return SortedIterable<V>(c, impl::getValToLessThan<V>(sortOn));
}

} // namespace obelisk