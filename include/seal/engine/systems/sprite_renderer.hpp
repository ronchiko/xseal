#pragma once

#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/ecs/system/traits/update.hpp"
#include "seal/engine/render/batch.hpp"
#include "seal/engine/render/pipeline.hpp"

#include "seal/engine/components/sprite.hpp"
#include "seal/engine/components/transform.hpp"

namespace seal::system {

	/**
	   The system resposible for drawing 2d sprites.
	 */
	class sprite_renderer
	{
	public:
		result<void> initialize();

		result<void> update_once();
		
		result<void> update(entity entity, const transform& transform, const sprite& sprite);

		result<void> update_last();

	private:
		struct sprite_queue
		{
			batch batch;
			size_t vertecies_used;
			std::shared_ptr<pipeline> pipeline; 

			bool is_available() const;
		};

		result<sprite_queue&> acquire_queue(const sprite& sprite);

		result<sprite_queue&> create_queue_for_sprite(const sprite& sprite, uintptr_t uuid);

		std::unordered_multimap<uintptr_t, sprite_queue> m_Queues;
	};
}
