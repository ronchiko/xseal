#pragma once

#include "seal/api/tagged_object.hpp"

namespace seal::gl {
	enum class tags : api::tag_t
	{
		Start = 1000,
		Program,
		Batch,
		Texture,
		UniformBuffer,
	};
}

