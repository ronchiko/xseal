#pragma once

#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/ecs/system/traits/update.hpp"

#include "seal/engine/components/sprite.hpp"
#include "seal/engine/components/transform.hpp"

namespace seal::system {

	/**
	   The system resposible for drawing 2d sprites.
	 */
	class sprite_renderer
	{
	public:
		void initialize();

		void update_once();
		
		void update(entity entity, const transform& transform, const sprite& sprite);

	private:
	};
}
