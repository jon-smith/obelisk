#pragma once

#include <vector>
#include <tuple>

namespace obelisk
{
	template <typename ...Types>
	class MultiVector
	{
	public:

		MultiVector() = default;

		MultiVector(size_t size)
		{
			// Todo - for each type, create vector
		}

		MultiVector(const std::vector<Types>&...args) :
			data(std::make_tuple(args...))
		{

		}

		MultiVector(std::vector<Types>&&...args) :
			data(std::make_tuple(std::forward<std::vector<Types>>(args(...))))
		{

		}

		size_t size() const
		{
			return std::get<0>(data).size();
		}

		bool isEmpty() const
		{
			return std::get<0>(data).empty();
		}

	private:
		std::tuple<std::vector<Types>...> data;
	};

	template <typename ...Types>
	MultiVector<Types...> MakeMultiVector(const std::vector<Types>&...args)
	{
		return MultiVector<Types...>(args...);
	}

	template <typename ...Types>
	class MultiVectorRef
	{
	public:

		MultiVectorRef(std::vector<Types>&...args) :
			dataRefs(std::tie(args...))
		{

		}

		size_t size() const
		{
			return std::get<0>(dataRefs).size();
		}

		bool isEmpty() const
		{
			return std::get<0>(dataRefs).empty();
		}

	private:
		std::tuple<std::vector<Types>&...> dataRefs;
	};

	template <typename ...Types>
	MultiVectorRef<Types...> MakeMultiVectorRef(std::vector<Types>&...args)
	{
		return MultiVectorRef<Types...>(args...);
	}

	template <typename ...Types>
	class MultiVectorCRef
	{
	public:

		MultiVectorCRef(const std::vector<Types>&...args) :
			dataRefs(std::tie(args...))
		{

		}

		size_t size() const
		{
			return std::get<0>(dataRefs).size();
		}

		bool isEmpty() const
		{
			return std::get<0>(dataRefs).empty();
		}

	private:
		std::tuple<const std::vector<Types>&...> dataRefs;
	};

	template <typename ...Types>
	MultiVectorCRef<Types...> MakeMultiVectorRef(const std::vector<Types>&...args)
	{
		return MultiVectorCRef<Types...>(args...);
	}

	template <typename ...Types>
	MultiVectorCRef<Types...> MakeMultiVectorCRef(const std::vector<Types>&...args)
	{
		return MultiVectorCRef<Types...>(args...);
	}
}