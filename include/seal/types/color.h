#pragma once

#include "seal/debug.h"
#include "seal/panic.h"
#include "seal/types.hpp"

namespace seal {
	class color : public v4<f32>
	{
	public:
		static constexpr f32 CHANNEL_MIN = 0.0f;
		static constexpr f32 CHANNEL_MAX = 1.0f;
		
		/**
		   Creates a color from RGB values (opaque).
		  
		   \param red: The red channel
		   \param green: The green channel
		   \param blue: The blue channel
		 */
		static constexpr color rgb(f32 red, f32 green, f32 blue) {
			return color({red, green, blue, CHANNEL_MAX});
		}
	
		/**
		   Creates a color from RGBA values.
		  
		   \param red: The red channel
		   \param green: The green channel
		   \param blue: The blue channel
		   \param alpha: The alpha channel
		 */
		static constexpr color rgba(f32 red, f32 green, f32 blue, f32 alpha) {
			return color({ red, green, blue, alpha });
		}

	private:
		constexpr color(v4<f32> channels)
			: v4<f32>(channels)
		{}

	};
}
