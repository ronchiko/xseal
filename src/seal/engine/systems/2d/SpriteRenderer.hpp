#pragma once

#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/render/batch.hpp"
#include "seal/engine/render/pipeline/pipeline.hpp"

#include "seal/engine/components/sprite.hpp"
#include "seal/engine/components/transform.hpp"

namespace seal::system {

	/**
	   The system responsible for drawing 2d sprites.
	 */
	class SpriteRenderer
	{
	public:
		void initialize();

		void update(entity entity, const transform& transform, const sprite& sprite);

		void update_last();

	private:
		struct SpriteQueue
		{
			batch batch;

			u32 texture_location;
			std::shared_ptr<texture2d> active_texture;

			size_t vertices_used;
			std::shared_ptr<pipeline> pipeline; 

			[[nodiscard]]
			bool is_available() const;
		};

		SpriteQueue& acquire_queue(const sprite& sprite);

		SpriteQueue& create_queue_for_sprite(const sprite& sprite, uintptr_t uuid);

		std::unordered_multimap<uintptr_t, SpriteQueue> m_Queues;
	};
}
