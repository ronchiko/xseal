#pragma once

#include "seal/types.hpp"

namespace seal {
	/**
		Class for implementing registering error values to be used
		with seal_is_error. 
		
		0 - For the interger types
		false - For boolean
		nullptr - For pointers
		
		Specialize for your own types.
	*/
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
	inline constexpr auto error_value_v = error_value<std::remove_cvref_t<T>>::VALUE;
}

/**
	Checks if values is its error value
*/
#define seal_is_error(...)                                                                         \
	static_cast<bool>((__VA_ARGS__) == (::seal::error_value_v<decltype(__VA_ARGS__)>))