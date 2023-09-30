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
	class sprite_renderer
	{
	public:
		void initialize();

		void update(entity entity, const transform& transform, const sprite& sprite);

		void update_last();

	private:
		struct sprite_queue
		{
			batch batch;

			u32 texture_location;
			std::shared_ptr<texture2d> active_texture;

			size_t vertices_used;
			std::shared_ptr<pipeline> pipeline; 

			[[nodiscard]]
			bool is_available() const;
		};

		sprite_queue& acquire_queue(const sprite& sprite);

		sprite_queue& create_queue_for_sprite(const sprite& sprite, uintptr_t uuid);

		std::unordered_multimap<uintptr_t, sprite_queue> m_Queues;
	};
}
