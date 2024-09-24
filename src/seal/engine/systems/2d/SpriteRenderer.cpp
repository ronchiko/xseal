#include "seal/engine/systems/2d/SpriteRenderer.hpp"

#include "seal/engine/render/pipeline/pipeline_data_provider.hpp"
#include "seal/types/id_provider.hpp"

static constexpr size_t VERTICES_PER_BATCH = 4096;

namespace seal::system {
	using vertex = api::vertex;

	void SpriteRenderer::initialize()
	{
		m_Queues.clear();

		auto ent = seal::entity::create();
		auto& sp = ent.add<sprite>();

		sp.tint = seal::WHITE;

		auto ent2 = seal::entity::create();
		auto& sp2 = ent2.add<sprite>();
		sp2.tint = seal::RED;
		sp2.slice.rect = UNIT_RECT;

		auto& trn = ent2.add<transform>();
		trn.position.x = -1;

		// auto re = sp.pipeline.acquire();
		// seal_verify_result(re);

		ent.add<transform>();
	}

	void SpriteRenderer::update(entity entity, const transform& transform, const sprite& sprite)
	{
		auto& queue = acquire_queue(sprite);
		auto [range, context] = queue.batch.lock<api::vertex>(api::batch_buffer_type::VertexBuffer);

		// Place the sprites vertices.
		// TODO: Enable multiple textures per pipeline!
		range[queue.vertices_used++] = vertex{
			transform.position,
			v2<f32>{ sprite.slice.rect.x, sprite.slice.rect.y },
			glm::one<v3<f32>>(),
			sprite.tint,
		};
		range[queue.vertices_used++] = vertex{
			transform.position + RIGHT,
			v2<f32>{ sprite.slice.rect.x + sprite.slice.rect.z, sprite.slice.rect.y },
			glm::one<v3<f32>>(),
			sprite.tint,
		};
		range[queue.vertices_used++] = vertex{
			transform.position + UP,
			v2<f32>{ sprite.slice.rect.x, sprite.slice.rect.y + sprite.slice.rect.w },
			glm::one<v3<f32>>(),
			sprite.tint,
		};
		range[queue.vertices_used++] = vertex{
			transform.position + UP + RIGHT,
			v2<f32>{ sprite.slice.rect.x + sprite.slice.rect.z,
					 sprite.slice.rect.y + sprite.slice.rect.w },
			glm::one<v3<f32>>(),
			sprite.tint,
		};
	}

	void SpriteRenderer::update_last()
	{
		// Publish all the queues
		for(auto& queue : m_Queues | std::views::values) {
			// Publish the queue and reset the its count.
			queue.pipeline->at(0).bind();

			const u32 texture_index = queue.active_texture->bind();
			queue.pipeline->at(0).update_uniform(queue.texture_location, texture_index);

			queue.batch.publish((queue.vertices_used >> 2) * 6);
			queue.vertices_used = 0;

			api::clear_bound_textures();
		}
	}

	SpriteRenderer::SpriteQueue& SpriteRenderer::acquire_queue(const sprite& sprite)
	{
		// The ID of a pipeline is its address in memory...
		auto& pipeline = sprite.pipeline.acquire();
		const auto uuid = reinterpret_cast<uintptr_t>(&pipeline);

		// Find all the queues that can fit our sprite.
		const auto [begin, end] = m_Queues.equal_range(uuid);
		if(begin == end) {
			// We haven't found anything.
			return create_queue_for_sprite(sprite, uuid);
		}

		// Search the queue for a queue that is both available and has the correct bound texture.
		const auto does_queue_match_sprite = [&sprite](const SpriteQueue& queue) {
			return queue.is_available(); // &&*queue.active_texture == sprite.slice.texture.acquire();
		};

		const auto range = std::ranges::subrange(begin, end) | std::views::values;
		const auto iterator = std::ranges::find_if(range, does_queue_match_sprite);

		if(std::end(range) == iterator) {
			// We found no available queues, allocate a new one
			return create_queue_for_sprite(sprite, uuid);
		}

		// We found a queue :)
		return std::ref(*iterator);
	}

	SpriteRenderer::SpriteQueue& SpriteRenderer::create_queue_for_sprite(const sprite& sprite,
																			uintptr_t uuid)
	{
		auto pipeline = sprite.pipeline.co_own();
		if(pipeline->count_stages() > 1) {
			throw seal::failure("sprite renderer only supports pipelines with a single stage.");
		}

		const auto [location, type] = pipeline->at(0).query_uniform("Texture");
		if(api::uniform_kind::Texture2d != type) {
			throw failure("Pipeline doesn't have a 'Texture' uniform that is a texture.");
		}

		auto batch = batch::create_triangular(VERTICES_PER_BATCH, true);


		SpriteQueue queue{
			std::move(batch), location, sprite.slice.texture.co_own(), 0, std::move(pipeline),
		};

		queue.batch.link_with_pipeline(queue.pipeline->at(0));

		return m_Queues.emplace(std::make_pair(uuid, std::move(queue)))->second;
	}

	bool SpriteRenderer::SpriteQueue::is_available() const
	{
		// We account for the fact that we require 4 vertices for a sprite.
		return vertices_used + 4 <= VERTICES_PER_BATCH;
	}
}
