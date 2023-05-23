#include "seal/engine/systems/sprite_renderer.hpp"

#include "seal/types/id_provider.hpp"

static constexpr size_t VERTECIES_PER_BATCH = 1024;

namespace seal::system {
	result<void> sprite_renderer::initialize()
	{
		auto ent = seal::entity::create();
		ent.add<sprite>();
		ent.add<transform>();

		return {};
	}

	result<void> sprite_renderer::update_once()
	{
		return {};
	}

	result<void> sprite_renderer::update(entity entity,
										 const transform& transform,
										 const sprite& sprite)
	{
		auto queue = acquire_queue(sprite);
		seal_verify_result(queue);

		auto is_ok = queue->batch.lock<api::vertex>(api::batch_buffer_type::VertexBuffer);
		seal_verify_result(is_ok);

		{
			auto [range, context] = std::move(*is_ok);
			
			// Place the sprites vertecies.
			range[queue->vertecies_used++] = api::vertex{ transform.position, v2<f32>{0, 0}, sprite.tint };
			range[queue->vertecies_used++] = api::vertex{ transform.position + RIGHT, v2<f32>{1, 0}, sprite.tint };
			range[queue->vertecies_used++] = api::vertex{ transform.position + UP, v2<f32>{0, 1}, sprite.tint };
			range[queue->vertecies_used++] = api::vertex{ transform.position + UP + RIGHT, v2<f32>{1, 1}, sprite.tint };
		}

		return {};
	}

	result<void> sprite_renderer::update_last() {
		// Publish all the queues
		for(auto& [_, queue] : m_Queues) {
			// Publish the queue and reset the its count.
			queue.pipeline->bind();

			queue.batch.publish((queue.vertecies_used >> 2) * 6);
			queue.vertecies_used = 0;
		}

		return {};
	}

	result<sprite_renderer::sprite_queue&> sprite_renderer::acquire_queue(const sprite& sprite)
	{
		auto sprite_pipline = sprite.shader.acquire();
		seal_verify_result(sprite_pipline);

		auto& pipeline = *sprite_pipline;
		auto uuid = reinterpret_cast<uintptr_t>(&pipeline);

		// Check if we even have batches that support our pipeline?
		auto iter = m_Queues.equal_range(uuid);
		if(iter.first == iter.second) {
			return create_queue_for_sprite(sprite, uuid);
		}

		// After we have all the possible suitable batches, find one that available.
		auto suitable = std::find_if(iter.first, iter.second, [](const auto& queue) {
			return queue.second.is_available();
		});
		// iter.second is the end
		if(iter.second == suitable) {
			// We found no available queues, allocate a new one
			return create_queue_for_sprite(sprite, uuid);
		}

		// We found a queue :)
		return std::ref(suitable->second);
	}

	// clang-format off
	seal::result<sprite_renderer::sprite_queue&> sprite_renderer::create_queue_for_sprite(
		const sprite& sprite, 
		uintptr_t uuid)
	{ // clang-format on

		auto batch = seal::batch::createTriangular(VERTECIES_PER_BATCH, true);
		seal_verify_result(batch);

		sprite_queue queue{
			*batch,
			0,
			sprite.shader.co_own(),
		};

		return std::ref(m_Queues.emplace(std::make_pair(uuid, std::move(queue)))->second);
	}

	bool sprite_renderer::sprite_queue::is_available() const {
		// We account for the fact that we require 4 vertecies for a sprite.
		return vertecies_used + 4 <= VERTECIES_PER_BATCH;
	}
}
