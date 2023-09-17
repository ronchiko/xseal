#pragma once

#include "seal/debug.h"
#include "seal/panic.h"
#include "seal/types.hpp"

namespace seal {
	/**
	   An object represnting a color with 4 channels (rgba).
	 */
	class color : public v4<f32>
	{
	public:
		using channel_t = f32;
		
		static constexpr channel_t CHANNEL_MIN = 0.0f;
		static constexpr channel_t CHANNEL_MAX = 1.0f;

		constexpr color() = default;

		/**
		   Creates a color from RGB values (opaque).

		   \param red: The red channel
		   \param green: The green channel
		   \param blue: The blue channel
		 */
		static constexpr color rgb(channel_t red, channel_t green, channel_t blue)
		{
			return color({ red, green, blue, CHANNEL_MAX });
		}

		/**
		   Creates a color from RGBA values.

		   \param red: The red channel
		   \param green: The green channel
		   \param blue: The blue channel
		   \param alpha: The alpha channel
		 */
		static constexpr color rgba(channel_t red, channel_t green, channel_t blue, channel_t alpha)
		{
			return color({ red, green, blue, alpha });
		}

	private:
		constexpr color(v4<channel_t> channels)
			: v4<f32>(channels)
		{}
	};

	/**
	   The color white.
	 */
	constexpr auto WHITE = color::rgb(color::CHANNEL_MAX, color::CHANNEL_MAX, color::CHANNEL_MAX);

	/**
	   The color black.
	 */
	constexpr auto BLACK = color::rgb(color::CHANNEL_MIN, color::CHANNEL_MIN, color::CHANNEL_MIN);

	/**
		The color RED
	 */
	constexpr auto RED = color::rgb(color::CHANNEL_MAX, color::CHANNEL_MIN, color::CHANNEL_MIN);

}
