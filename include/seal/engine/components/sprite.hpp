#pragma once

#include "seal/types/color.hpp"

#include "seal/engine/resource.hpp"

namespace seal {
	class pipeline;

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
		resource_ref<pipeline> shader = R"(::\SpritePipeline.json)";
	};
}
