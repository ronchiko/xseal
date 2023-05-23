#pragma once

#include <type_traits>

#include "seal/types/result.hpp"

namespace seal {

	template<typename T>
	struct is_result : public std::false_type
	{};

	template<typename OkT, typename ErrT>
	struct is_result<result<OkT, ErrT>> : public std::true_type
	{};

	template<typename TypeT>
	inline constexpr bool is_result_v = is_result<TypeT>::value;

	template<typename TypeT>
	inline constexpr bool is_void_result_v = std::is_same_v<TypeT, result<void>>;
}
