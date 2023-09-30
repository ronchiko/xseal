#pragma once

#include <tuple>

namespace seal {
	/**
	   Puts a type as the first one inside a tuple.

	   \tparam T: The type to push
	   \tparam Tuple: The tuple to push into
	 */
	template<typename T, typename Tuple>
	struct tuple_push_front;

	template<typename T, typename... Args>
	struct tuple_push_front<T, std::tuple<Args...>>
	{
		using type = std::tuple<T, Args...>;
	};

	template<typename Type, typename Tuple>
	using tuple_push_front_t = tuple_push_front<Type, Tuple>::type;

	namespace detail {
		template<size_t Target, size_t Index, typename... Args>
		class remove_tuple_element;

		template<size_t T, size_t I>
		class remove_tuple_element<T, I>
		{
		public:
			using type = std::tuple<>;
		};
		
		/// Internal implmentation of tuple_remove_element
		template<size_t Target, size_t Index, typename Current, typename... Args>
		class remove_tuple_element<Target, Index, Current, Args...>
		{
		private:
			using next_tuple = remove_tuple_element<Target, Index + 1, Args...>::type;

			using build_tuple = tuple_push_front_t<Current, next_tuple>;

		public:
			using type = std::conditional_t<Target == Index, std::tuple<Args...>, build_tuple>;
		};
	}

	/**
	   Removes an element at an index from a tuple.

	   \tparam Index - The index of the element to remove.
	   \tparam Tuple - The tuple type to remove the element from.
	 */
	template<size_t Index, typename Tuple>
	struct tuple_remove_element;

	template<size_t Index, typename... Rest>
	struct tuple_remove_element<Index, std::tuple<Rest...>>
		: detail::remove_tuple_element<Index, 0, Rest...>
	{
	private:
		static_assert(Index < sizeof...(Rest), "Asking for an index outside of the tuple's size");
	};

	template<size_t Index, typename Tuple>
	using tuple_remove_element_t = typename tuple_remove_element<Index, Tuple>::type;

	/**
	   Removes the first element from a tuple.
	 */
	template<typename Tuple>
	using tuple_remove_first_element_t = tuple_remove_element_t<0, Tuple>;

	/*template<size_t Index, typename NewType, typename... Elements>
	constexpr detail::transform_tuple_element<Index, 0, NewType, Elements...>::type
	transform_element(const std::tuple<Elements...>& tuple, std::function<NewType()>)
	{
		using result_t = detail::transform_tuple_element<Index, 0, NewType, Elements...>::type;

		result_t new_tuple{};
		
	}*/
}
