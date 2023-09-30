#pragma once

#include <concepts>
#include <type_traits>

#include "seal/types/traits/first_of.hpp"

namespace seal {

	namespace detail {
		template<typename CompareT, typename... List>
		struct all_of;

		template<typename CompareT>
		struct all_of<CompareT> : public std::bool_constant<true>
		{};

		template<typename CompareT, typename CurrentT, typename... RemainedersT>
		struct all_of<CompareT, CurrentT, RemainedersT...>
			: public std::conditional_t<std::is_same_v<CompareT, CurrentT>,
										all_of<CompareT, RemainedersT...>,
										std::bool_constant<false>>
		{};
	}

	/**
	   Checks if a list of types all match a certain type.
	 */
	template<typename CompareT, typename... List>
	concept all_of = detail::all_of<CompareT, List...>::value;

	/**
	   Checks if all types in a list of types are the same.
	 */
	template<typename... List>
	concept all_same = detail::all_of<seal::first_of_t<List...>, List...>::value;
}
