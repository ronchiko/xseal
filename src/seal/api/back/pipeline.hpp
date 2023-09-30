#pragma once

#include <algorithm>
#include <variant>

#include "seal/types.hpp"
#include "seal/types/conversion_table.hpp"

#include "seal/api/resource.hpp"
#include "seal/api/tagged_object.hpp"

namespace seal::api {

	namespace shader {
		// Attributes
		inline constexpr const char *VERTEX_PARAM_NAME = "Seal_Vertex";
		inline constexpr const char *UV_PARAM_NAME = "Seal_UV";
		inline constexpr const char *TINT_PARAM_NAME = "Seal_Tint";
	}

	/**
		The allowed types a uniform might hold.
	 */
	enum class uniform_kind
	{
		Texture2d,
		Buffer,
	};

	/**
		The concrete value of the uniform.
	 */
	using uniform_value = std::variant<u32, abstract_t>;

	/**
		The fields needed to define a new uniform for a stage.
	 */
	struct uniform_declaration_options
	{
		uniform_kind kind;			 // The type of the value the uniform holds.
		std::string uniform_name;	 // The name of the uniform to allow queries.
		u32 expected_location;		 // The expected location the uniform is meant to be at.
		uniform_value default_value; // The initial value the uniform should hold.
	};

	/**
		The fields needed to update a uniforms value.
	 */
	struct uniform_update_options
	{
		u32 location;				 // The location of the uniform
		uniform_value updated_value; // The new value of the uniform, must be of same type it was
									 // declared with.
	};

	/**
		Query information about a uniform
	 */
	struct uniform_query_information
	{
		u32 location;	   // The location of the uniform.
		uniform_kind kind; // What type of value it holds.
	};

	enum class pipeline_stage_type
	{
		GraphicsPipeline,
		ComputationPipeline
	};

	/**
	   Describes a pipeline.
	 */
	struct pipeline_stage_creation_options
	{
		// Description for a graphics pipeline
		struct graphics
		{
			resource vertex;
			resource fragment;
		};

		// Description for a compute pipeline
		struct compute
		{
			resource compute_shader;
		};

		pipeline_stage_type type;
		std::variant<graphics, compute> description;
		std::vector<uniform_declaration_options> uniforms;
	};

	/**
	   Creates a new pipeline.
	 */
	abstract_t create_pipeline_stage(const pipeline_stage_creation_options& desc);

	/**
	   Activate a pipeline (sets it as the current pipeline).

	   \param pipeline: The id of the pipeline to bind
	 */
	void bind_pipeline_stage(abstract_t pipeline);

	/**
	   Frees a pipeline.

	   \param pipeline: The id of the pipeline to free
	 */
	void free_pipeline_stage(abstract_t pipeline);

	/**
		Queries information from a pipeline stage about a uniform.

		\param pipeline: The pipeline to update.
		\param name: The name of the uniform.

		\return: The location and type of the un
	 */
	uniform_query_information query_pipeline_uniform(abstract_t pipeline, const std::string& name);

	/**
		Updates the value of a uniform inside a pipeline stage.

		\param pipeline: The pipeline to update.
		\param update: The updated value of the uniform.
	 */
	void update_pipeline_uniform(abstract_t pipeline, uniform_update_options update);
}

// clang-format off

SEAL_BI_CONVERSION_TABLE(seal::api::pipeline_stage_type, std::string, {
	{api::pipeline_stage_type::GraphicsPipeline, "graphics"},
	{api::pipeline_stage_type::ComputationPipeline, "compute"},
});

SEAL_BI_CONVERSION_TABLE(seal::api::uniform_kind, std::string, {
	{api::uniform_kind::Texture2d, "texture2d"},
	{api::uniform_kind::Buffer, "external-buffer"},
});

// clang-format on