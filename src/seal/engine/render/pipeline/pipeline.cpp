#include "seal/engine/render/pipeline/pipeline.hpp"

#include <ranges>

#include "seal/engine/render/pipeline/pipeline_data_provider.hpp"
#include "seal/engine/render/texture2d.hpp"
#include "seal/engine/resource.hpp"
#include "seal/opt/json.hpp"
#include "seal/types/ranges/utility.hpp"

namespace seal {

	using pipeline_type = api::pipeline_stage_type;

	namespace {
		class pipeline_json_creator
		{
		public:
			/**
				Creates a pipeline resource from a JSON schema.
			 */
			[[nodiscard]] std::shared_ptr<pipeline> from_json(const nlohmann::json& j)
			{
				const auto pipeline_type = json::parse_to_enum<seal::pipeline_type>(j["type"]);
				const nlohmann::json& stages_array = j["stages"];

				const auto create_pipeline = [&](const auto& stage) {
					switch(pipeline_type) {
					case pipeline_type::GraphicsPipeline:
						return load_graphics_stage(stage);
					case pipeline_type::ComputationPipeline:
					default:
						throw failure("Unsupported pipeline type: {}",
									  convert<std::string>(pipeline_type));
					}
				};

				auto stages = stages_array | std::views::transform(create_pipeline);
				return std::make_shared<pipeline>(pipeline_type,
												  std::vector(stages.begin(), stages.end()));
			}

		private:
			/**
				Creates a pipeline stage from a JSON matching that schema.

				\param j: The item of the JSON.
			 */
			pipeline_stage load_graphics_stage(const nlohmann::json& j)
			{
				const auto& vertex = j["vertex"].get<std::string>();
				const auto& fragment = j["fragment"].get<std::string>();

				const auto& uniforms_object = j["uniforms"];
				std::vector<api::uniform_declaration_options> uniforms;
				std::transform(uniforms_object.items().begin(),
							   uniforms_object.items().end(),
							   std::back_inserter(uniforms),
							   [&](const auto& u) {
								   return create_uniform_object(u.key(), u.value());
							   });

				api::pipeline_stage_creation_options::graphics graphics{
					load_resource(vertex),
					load_resource(fragment),
				};

				return pipeline_stage::create(std::move(graphics), std::move(uniforms));
			}

			/**
				Creates a uniform object given its name and JSON schema

				\param name: The name of the uniform
				\param j: The information about the item
			 */
			api::uniform_declaration_options create_uniform_object(const std::string& name,
																   const nlohmann::json& j)
			{
				if(!j.is_object()) {
					throw seal::failure("Uniform is expected to be an object");
				}
				const auto uniform_kind = json::parse_to_enum<api::uniform_kind>(j["type"]);

				api::uniform_declaration_options uniform{
					uniform_kind,
					name,
					j["location"].get<u32>(),
					create_uniform_value(uniform_kind, j["value"]),
				};

				return uniform;
			}

			api::uniform_value create_uniform_value(const api::uniform_kind kind,
													const nlohmann::json& j)
			{
				switch(kind) {
				case api::uniform_kind::Texture2d: // BUG: What we do here?
					return 0u;
				case api::uniform_kind::Buffer:
					return pipeline_data_provider<typeless_provider>::open(j.get<std::string>())
						.abstracted();
				}

				throw seal::failure("Invalid enum kind {}", static_cast<u32>(kind));
			}
		};
	}

	pipeline::pipeline(const pipeline_type type, std::vector<pipeline_stage> stages)
		: m_Type(type)
		, m_Stages(std::move(stages))
	{}

	std::shared_ptr<pipeline> resource_loader<pipeline>::load(const resource& resource)
	{
		const auto pipeline_json = json::load_as_json(resource);
		return pipeline_json_creator().from_json(pipeline_json);
	}
}
