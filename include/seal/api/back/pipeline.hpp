#pragma once

#include <variant>

#include "seal/types.hpp"
#include "seal/types/result.hpp"

#include "seal/api/tagged_object.hpp"
#include "seal/api/resource.hpp"

namespace seal::api {

	namespace shader {
		// Attributes
		inline constexpr const char* VERTEX_PARAM_NAME = "Seal_Vertex";
		inline constexpr const char *UV_PARAM_NAME = "Seal_UV";
		inline constexpr const char *TINT_PARAM_NAME = "Seal_Tint";
	}

	/**
	   Describes a pipeline.
	 */
	struct pipeline_description
	{
		enum type_t
		{
			GraphicsPipeline,
			ComputationPipeline
		};
		
		// Description for a graphics pipeline
		struct graphics
		{
			resource vertex;
			resource fragment;
		};

		// Description for a compute pipeline
		struct compute
		{
			resource compute;
		};

		type_t type;
		std::variant<graphics, compute> description;
	};

	/**
	   Creates a new pipeline.
	 */
	result<abstract_t> create_pipeline(const pipeline_description& desc);

	/**
	   Activate a pipeline (sets it as the current pipeline).

	   \param pipeline: The id of the pipeline to bind
	 */
	result<void> bind_pipeline(abstract_t pipeline);

	/**
	   Frees a pipeline.

	   \param pipline: The id of the pipeline to free
	 */
	void free_pipeline(abstract_t pipeline);
}
