#pragma once

#include <type_traits>

namespace seal {
	template<typename T, typename...>
	struct is_one_of;

	template<typename T, typename Other>
	struct is_one_of<T, Other> : std::bool_constant<std::is_same_v<T, Other>>
	{};

	template<typename T, typename Other, typename... Rest>
	struct is_one_of<T, Other, Rest...>
		: std::conditional_t<std::is_same_v<T, Other>, std::true_type, is_one_of<T, Rest...>>
	{};

	/**
	 * Checks if a type is one of the types in a list of types.
	 */
	template<typename T, typename... Types>
	constexpr bool is_one_of_v = is_one_of<T, Types...>::value;

	template<typename T, typename... Types>
	concept one_of = is_one_of_v<T, Types...>;
}
