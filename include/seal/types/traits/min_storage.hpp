#pragma once

#include <limits>

#include "seal/types.hpp"

namespace seal {

	/**
	   Picks the minimal size required to host an integer value.
	 */
	template<u64 Value>
	struct min_storage
	{
		using type_t = std::conditional_t<
			(Value <= std::numeric_limits<u8>::max()),
			u8,
			std::conditional_t<(Value <= std::numeric_limits<u16>::max()),
							   u16,
							   std::conditional_t<(Value <= std::numeric_limits<u32>::max()), u32, u64>>>;
	};

	/**
	   Picks the minimal size required to host a floating point value.
	 */
	template<f64 Value>
	struct min_floating_storage
	{
		using type_t = std::conditional_t<(Value <= std::numeric_limits<f32>::max()), f32, f64>;
	};

	template<u64 Value>
	using min_storage_t = min_storage<Value>::type_t;

	template<f64 Value>
	using min_floating_storage_t = min_floating_storage<Value>::type_t;

	static_assert(std::is_same_v<min_storage_t<243>, u8>, "Wrong storage selection");
}
