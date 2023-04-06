#include "seal/api/back/pipeline.hpp"

#include <unordered_map>

#include "pipeline/shader.hpp"
#include "pipeline/program.hpp"

using namespace seal::gles3;

namespace seal {

	static std::unordered_map<id, gles3::program> g_LoadedPrograms = {};

	namespace api {
		result<id> create_pipeline(const pipeline_description& desc)
		{
			if(desc.type != pipeline_description::GraphicsPipeline) {
				return seal::fail<failure::NotImplemented>();
			}

			auto& description = std::get<pipeline_description::graphics>(desc.description);

			// Turn the resoiurces into shaders.
			auto vertex = shader::from_resource(description.vertex, shader::type::Vertex);
			seal_verify_result(vertex);

			auto fragment = shader::from_resource(description.fragment, shader::type::Fragement);
			seal_verify_result(fragment);

			// Link the shaders into a program
			shader shaders[] = { std::move(*vertex), std::move(*fragment) };
			auto prog = program::link(std::span{ shaders });
			seal_verify_result(prog);

			// Move the program into the global storage
			const auto program_id = prog->id();
			g_LoadedPrograms[program_id] = std::move(*prog);

			return program_id;
		}

		result<void> bind_pipeline(id pipeline) {
			// Since we return the gl id of our pipeline we can just bind the program id.
			glUseProgram(pipeline);
			return {};
		}

		void free_pipeline(id pipeline) {
			auto iter = g_LoadedPrograms.find(pipeline);
			if (iter != g_LoadedPrograms.end()) {
				iter->second = nullptr;
			}
		}
	}
}