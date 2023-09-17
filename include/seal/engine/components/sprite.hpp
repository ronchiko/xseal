#pragma once

#include "seal/types/color.hpp"

#include "seal/engine/render/pipeline/pipeline.hpp"
#include "seal/engine/render/texture2d.hpp"
#include "seal/engine/resource.hpp"

namespace seal {
	struct sprite_slice
	{
		resource_ref<texture2d> texture = R"(::\EngineLogo.png)";
		v4<f32> rect = seal::UNIT_RECT;
	};

	struct sprite
	{
		union
		{
			u8 flags = 0;

			struct
			{
				// If this is 0 this sprite's alpha channels are ignored.
				u8 transparent:1;
			};
		};

		color tint = seal::WHITE;
		resource_ref<pipeline> pipeline = R"(::\2d\Default.ppln)";
		sprite_slice slice = { R"(::\EngineLogo.png)", seal::UNIT_RECT };
	};
}
