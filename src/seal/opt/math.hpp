#pragma once

#include <concepts>

#include "seal/types.hpp"
#include "seal/types/traits/numeric.hpp"

namespace seal {
	/**
	   Aligns a value upwards.

	   \param value: The value to align
	   \param alignment: The value to align to
	 */
	constexpr auto align(std::integral auto value, std::integral auto alignment) noexcept
	{
		return (value / alignment + 1) * alignment;
	}

	/**
	   Same as seal::align, but aligns downwards.
	 */
	constexpr auto align_down(std::integral auto value, std::integral auto alignment) noexcept {
		return (value / alignment) * alignment;	// Notice the integer division.
	}

	/**
	   Checks if a value is aligned to another value.
	  
	   \param value: The value compare to the alignment.
	   \param alignment: The alignment to check for.

	   \return: True if the value is aligned to expceted alignment.
	 */
	constexpr auto is_aligned(std::integral auto value, std::integral auto alignment) noexcept {
		return !static_cast<bool>(value % alignment);
	}

	/**
	   Clamps a number between a minimum and a maximum.
	  
	   \param value: The value to clamp
	   \param min: The minimum value
	   \param max: The maximum value
	   \return 
	 */
	template<seal::numeric T>
	constexpr auto clamp(T value, T min, T max) noexcept
	{
		return value < min ? min : (value > max ? max : value);
	}

	/**
	   Creates a with a bit in an offset.
	  
	   \param bit: The bit to activate
	 */
	template<std::integral T = u32>
	constexpr T bit(u8 bit) noexcept
	{
		return static_cast<T>(1) << bit;
	}

	/**
	   Creates a bitmask.
	  
	   \param length: The length of the bitmask
	   \param start: The first active bit of the bitmask
	 */
	template<std::integral T = u32>
	constexpr T bitmask(u8 length, u8 start = 0) noexcept
	{
		return (seal::bit(length) - 1) << start;
	}
}
