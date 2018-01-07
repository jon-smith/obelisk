#pragma once

#include <functional>

#include "Optional.h"

namespace obelisk
{
	template <typename ReturnT>
	class LazyEval
	{
	public:

		template <typename Fn>
		LazyEval(Fn &&fn) : fn(fn)
		{

		}

		ReturnT get() const
		{
			if (!value.hasValue())
				value.set(fn());
			return value.value();
		}

	private:
		mutable Optional<ReturnT> value;
		std::function<ReturnT(void)> fn;
	};
}