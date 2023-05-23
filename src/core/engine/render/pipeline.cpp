#include "seal/engine/render/pipeline.hpp"

#include "seal/api/back/pipeline.hpp"
#include "seal/types/error_value.hpp"

using namespace seal::api;

namespace seal {

	pipeline::~pipeline() noexcept
	{
		release();
	}

	void pipeline::bind() const
	{
		seal::api::bind_pipeline(m_Id);
	}

	result<pipeline> pipeline::create_graphics(api::pipeline_description::graphics graphics)
	{
		api::pipeline_description description{
			api::pipeline_description::GraphicsPipeline,
			graphics,
		};

		auto new_pipeline = api::create_pipeline(description);
		seal_verify_result(new_pipeline);

		return pipeline{ *new_pipeline };
	}

	bool pipeline::operator==(const pipeline& other) const
	{
		return other.m_Id == m_Id;
	}

	bool pipeline::operator!=(const pipeline& other) const
	{
		return other.m_Id != m_Id;
	}

	void pipeline::release() noexcept
	{
		if(seal_is_error(m_Id)) {
			return;
		}

		seal_mute_exceptions({ seal::api::free_pipeline(m_Id); });
	}

	seal::result<std::shared_ptr<pipeline>> seal::resource_loader<pipeline>::load(resource& resource)
	{
		auto resource_json = json::load_as_json(resource);
		seal_verify_result(resource_json);

		auto type = (*resource_json)["type"].get<std::string>();

		switch(parse_type_string(type)) {
		case api::pipeline_description::GraphicsPipeline:
		{
			auto vertex = (*resource_json)["vertex"].get<std::string>();
			auto fragment = (*resource_json)["fragment"].get<std::string>();

			auto vertex_resource = load_resource(vertex);
			seal_verify_result(vertex_resource);

			auto fragment_resource = load_resource(fragment);
			seal_verify_result(fragment_resource);

			auto pipeline = pipeline::create_graphics(api::pipeline_description::graphics{
				std::move(*vertex_resource),
				std::move(*fragment_resource),
			});
			
			seal_verify_result(pipeline);
			return std::make_shared<seal::pipeline>(std::move(*pipeline));
		}

		case pipeline_description::ComputationPipeline:
			seal::log::error("Pipeline deserialization for type '{}' is not supported", type);
			[[fallthrough]]; 
		default:
			return seal::fail<seal::failure::NotImplemented>();
		}
	}

	pipeline_description::type_t resource_loader<pipeline>::parse_type_string(const std::string& type)
	{
		static const std::unordered_map<std::string, pipeline_description::type_t> s_Types{
			{ "graphics", pipeline_description::GraphicsPipeline },
			{ "compute", pipeline_description::ComputationPipeline },
		};

		auto value = s_Types.find(type);
		if(s_Types.end() == value) {
			seal::log::warning("Unknown pipeline type: {}", type);
			return static_cast<pipeline_description::type_t>(-1);
		}

		return value->second;
	}
}
