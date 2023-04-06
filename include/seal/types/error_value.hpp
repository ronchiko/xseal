#pragma once

#include "seal/types.hpp"


namespace seal {
	template<typename T>
	struct error_value;

	template<>
	struct error_value<bool>
	{
		constexpr static bool VALUE = false;
	};

	template<>
	struct error_value<i32>
	{
		constexpr static i32 VALUE = 0;
	};

	template<>
	struct error_value<u32>
	{
		constexpr static u32 VALUE = 0;
	};

	template<>
	struct error_value<f32>
	{
		constexpr static f32 VALUE = 0;
	};

	template<typename T>
	struct error_value<T *>
	{
		constexpr static T *VALUE = nullptr;
	};

	template<typename T>
	constexpr decltype(error_value<std::remove_cvref_t<T>>::VALUE)
		error_value_v = error_value<std::remove_cvref_t<T>>::VALUE;
}
