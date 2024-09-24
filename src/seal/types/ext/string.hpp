#pragma once

#include <iterator>

#include "seal/types.hpp"
#include "seal/types/failure.hpp"

namespace seal::string
{
	/**
		Converts a string into an integer.

		\param view: The string to turn into an integer.
	 */
	template<typename IntegerT, typename CharT>
	constexpr IntegerT to_int(const std::basic_string_view<CharT> view)
	{
		// If we are a signed integer check for negative numbers...
		if constexpr(std::is_signed_v<IntegerT>) {
			if(!view.empty() && '-' == view[0]) {
				// Remove glyph and negate the result.
				return -to_int<IntegerT>(view.substr(1));
			}
		}

		// Keep to values to track overflows.
		IntegerT value = 0, previous_value = 0;
		for(const auto& c : view)
		{
			// Digit check
			if(c < '0' || c > '9')
			{
				throw failure("Expected a digit!");
			}

			previous_value = value;
			value = value * 10 + (c - '0');

			// Overflow check.
			if(value < previous_value)
			{
				throw failure("Number exceed the limits of the integer!");
			}
		}

		return value;
	}

	/**
		Parses a string into a f32, if possible.

		\param string: The string to parse.
	 */
	template<typename CharT>
	constexpr f32 to_f32(const std::basic_string_view<CharT> string)
	{
		auto split_offset = string.find('.');
		if(std::basic_string_view<CharT>::npos == split_offset)
		{
			split_offset = string.size();
		}

		// Find the full part and the fraction of the floating point number.
		auto full_part = to_int<i32>(string.substr(0, split_offset));
		if(split_offset >= string.size())	// Just whole integer
		{
			return static_cast<f32>(full_part);
		}

		// Find the fraction part and keep dividing by 10 until we are less then 0.
		f32 fraction = static_cast<f32>(to_int<u32>(string.substr(split_offset + 1)));
		while(fraction >= 1) {
			fraction /= 10.f;
		}

		return static_cast<f32>(full_part) + fraction;
	} 

}
