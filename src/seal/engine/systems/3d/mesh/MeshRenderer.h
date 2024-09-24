#pragma once

#include "seal/engine/ecs/entity.hpp"

#include "seal/engine/components/mesh.hpp"
#include "seal/engine/components/transform.hpp"

namespace seal::system
{
	class MeshRenderer
	{
	public:
		void initialize();

		void update(entity entity, const transform& transform, const mesh& sprite);

	private:

	};
}
