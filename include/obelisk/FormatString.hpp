#pragma once

#include <memory>
#include <string>

#include "obelisk/ObeliskMeta.hpp"

namespace obelisk
{
namespace impl
{
template <typename... Args> struct ContainsAnyStdStringTypes
{
	static constexpr bool value = ContainsAnyOf<std::wstring, Args...>::value || ContainsAnyOf<const std::wstring, Args...>::value ||
								  ContainsAnyOf<std::wstring &, Args...>::value || ContainsAnyOf<const std::wstring &, Args...>::value ||
								  ContainsAnyOf<std::string, Args...>::value || ContainsAnyOf<const std::string, Args...>::value ||
								  ContainsAnyOf<std::string &, Args...>::value || ContainsAnyOf<const std::string &, Args...>::value;
};
} // namespace impl

// From https://stackoverflow.com/questions/2342162/stdstring-formatting-like-sprintf
template <typename... Args> std::string formatString(const std::string &format, Args... args)
{
	// Assert on any std strings or wchar_t types
	static_assert(impl::ContainsAnyStdStringTypes<Args...>::value == false, "");
	static_assert(ContainsAnyOf<wchar_t *, Args...>::value == false, "");
	static_assert(ContainsAnyOf<const wchar_t *, Args...>::value == false, "");

	// Replace %S with %s as we only allow char args
	if (format.find("%S") != std::string::npos)
	{
		auto formatCopy = format;
		for (size_t i = 1; i < format.size(); ++i)
		{
			if (formatCopy[i - 1] == '%' && formatCopy[i] == 'S')
				formatCopy[i] = 's';
		}
		return formatString(formatCopy, args...);
	}

	const auto size = std::snprintf(nullptr, 0, format.c_str(), args...) + 1;
	if (size < 1)
		return {};

	std::unique_ptr<char[]> buf(new char[size]);
	snprintf(buf.get(), static_cast<size_t>(size), format.c_str(), args...);
	return std::string(buf.get(), buf.get() + static_cast<size_t>(size) - 1);
}

namespace impl
{
template <typename... Args> std::wstring recursiveBufferIncreaseFormatString(size_t bufferSize, const std::wstring &format, Args... args)
{
	try
	{
		std::unique_ptr<wchar_t[]> buf(new wchar_t[bufferSize]);
		const auto writtenSize = std::swprintf(buf.get(), bufferSize, format.c_str(), args...);

		if (writtenSize >= 0 && writtenSize < bufferSize)
		{
			return std::wstring(buf.get(), buf.get() + static_cast<size_t>(writtenSize));
		}

		return recursiveBufferIncreaseFormatString(bufferSize * 2, format, args...);
	}
	catch (const std::exception &)
	{
		return std::wstring{};
		}
}
} // namespace impl

template <typename... Args> std::wstring formatString(const std::wstring &format, Args... args)
{
	// Assert on any std strings or char type
	static_assert(impl::ContainsAnyStdStringTypes<Args...>::value == false, "");
	static_assert(ContainsAnyOf<char *, Args...>::value == false, "");
	static_assert(ContainsAnyOf<const char *, Args...>::value == false, "");

// if we're not on MSVC
// check that we are only using %S for string
// then recursively try increasing size buffers
#ifndef _MSC_VER_
	if (format.find(L"%s") != std::string::npos)
	{
		auto formatCopy = format;
		for (size_t i = 1; i < format.size(); ++i)
		{
			if (formatCopy[i - 1] == '%' && formatCopy[i] == 's')
				formatCopy[i] = 'S';
		}
		return formatString(formatCopy, args...);
	}

	return impl::recursiveBufferIncreaseFormatString(format.size() + 1, format, args...);

// On MSVC, we can call swprintf to get the size - doesn't work on other compilers
#else
	const auto size = std::swprintf(nullptr, 0, format.c_str(), args...) + 1;
	if (size < 1)
		return {};

	std::unique_ptr<wchar_t[]> buf(new wchar_t[size]);
	std::swprintf(buf.get(), static_cast<size_t>(size), format.c_str(), args...);
	return std::wstring(buf.get(), buf.get() + static_cast<size_t>(size) - 1);
#endif
}

} // namespace obelisk
