#pragma once

#include <concepts>
#include <type_traits>

#include "seal/types/traits/all_of.hpp"

namespace seal {
	template<typename Type>
	concept enum_type = std::is_enum_v<Type>;

	namespace detail {

		template<enum_type FirstT>
		constexpr std::underlying_type_t<FirstT> join_enums(FirstT first)
		{
			return static_cast<std::underlying_type_t<FirstT>>(first);
		}
		
		// Generic function doesn't handle the case where we have only one enum.
		template<enum_type FirstT, enum_type... EnumT>
		constexpr std::underlying_type_t<FirstT> join_enums(FirstT first, EnumT... values)
		{
			using underlying_t = std::underlying_type_t<FirstT>;

			return static_cast<underlying_t>(first) | join_enums(values...);
		}
	}

	/**
	   Performs binary or on a list of enums from the same type.
	 */
	template<enum_type... EnumT>
	requires all_same<EnumT...> && (sizeof...(EnumT) > 0) // Ensure all the enums have the same type
	constexpr first_of_t<EnumT...> join_enums(EnumT... values)
	{
		using first_t = first_of_t<EnumT...>;
		using underlying_t = std::underlying_type_t<first_t>;

		return static_cast<first_t>(detail::join_enums(values...));
	}
}
