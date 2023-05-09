#include "seal/api/back/pipeline.hpp"

#include <unordered_map>

#include "seal/types/ext/variant.hpp"
#include "seal/types/storage.hpp"

#include "pipeline/program.hpp"
#include "pipeline/shader.hpp"

inline static seal::storage<seal::gl::program> g_LoadedPrograms{};

namespace seal::api {
	result<abstract_t> create_pipeline(const pipeline_description& desc)
	{
		if(desc.type != pipeline_description::GraphicsPipeline) {
			// Non-Graphics pipeline are not supported yet.
			return seal::fail<failure::NotImplemented>();
		}

		auto description = seal::try_get<pipeline_description::graphics>(desc.description);
		seal_verify_result(description);

		// Turn the resoiurces into shaders.
		auto vertex = gl::shader::from_resource((*description).vertex, gl::shader::type::Vertex);
		seal_verify_result(vertex);

		auto frag = gl::shader::from_resource((*description).fragment, gl::shader::type::Fragement);
		seal_verify_result(frag);

		// Link the shaders into a program
		gl::shader shaders[] = { std::move(*vertex), std::move(*frag) };
		auto program = gl::program::link(std::span{ shaders });
		seal_verify_result(program);

		// Move the program into the global storage
		return abstract_t::bind(g_LoadedPrograms.store(std::move(*program)));
	}

	result<void> bind_pipeline(abstract_t pipeline)
	{
		// Since we return the gl id of our pipeline we can just bind the program id.
		auto *obj = pipeline.acquire<seal::gl::program>();

		obj->bind();
		return {};
	}

	void free_pipeline(abstract_t pipeline)
	{
		// Acquire the pipeline pointer and erase it.
		auto *obj = pipeline.acquire<seal::gl::program>();

		g_LoadedPrograms.erase(*obj);
	}
}
