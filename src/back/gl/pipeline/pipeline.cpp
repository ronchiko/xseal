#include "seal/api/back/pipeline.hpp"

#include "seal/types/ext/variant.hpp"
#include "seal/types/storage.hpp"

#include "pipeline/program.hpp"
#include "pipeline/shader.hpp"

namespace {
	inline seal::storage<seal::gl::program> g_LoadedPrograms{};
}

namespace seal::api {
	abstract_t create_pipeline_stage(const pipeline_stage_creation_options& desc)
	{
		if(desc.type != pipeline_stage_type::GraphicsPipeline) {
			// Non-Graphics pipeline are not supported yet.
			throw seal::fail<failure::NotImplemented>();
		}

		const auto& description = seal::try_get<pipeline_stage_creation_options::graphics>(desc.description);

		// Turn the resources into shaders.
		auto vertex = gl::shader::from_resource(description.vertex, gl::shader::type::Vertex);
		auto frag = gl::shader::from_resource(description.fragment, gl::shader::type::Fragment);

		// Link the shaders into a program
		gl::shader shaders[] = { std::move(vertex), std::move(frag) };
		auto program = gl::program::link(std::span(shaders), desc);
		// Move the program into the global storage
		auto& pipeline = g_LoadedPrograms.store(std::move(program));
		return abstract_t::bind(&pipeline);
	}

	void bind_pipeline_stage(abstract_t pipeline)
	{
		// Since we return the gl id of our pipeline we can just bind the program id.
		auto *obj = pipeline.acquire<seal::gl::program>();

		obj->bind();

#if defined(SEAL_GL_NO_VAO)
		// In ES3 there are no VAO so we have to bind the information every time.
		obj->bind_shader_information(0);
#endif
	}

	void free_pipeline_stage(abstract_t pipeline)
	{
		// Acquire the pipeline pointer and erase it.
		auto *obj = pipeline.acquire<seal::gl::program>();

		g_LoadedPrograms.erase(*obj);
	}

	uniform_query_information query_pipeline_uniform(abstract_t pipeline, const std::string& name)
	{
		const auto *obj = pipeline.acquire<gl::program>();

		return obj->query(name);
	}

	void update_pipeline_uniform(abstract_t pipeline, uniform_update_options update)
	{
		const auto *obj = pipeline.acquire<gl::program>();
		obj->set_uniform(std::move(update));
	}


}
