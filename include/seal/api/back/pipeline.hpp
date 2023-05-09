#pragma once

#include <variant>

#include "seal/types.hpp"
#include "seal/types/result.hpp"

#include "seal/api/tagged_object.hpp"
#include "seal/api/resource.hpp"

namespace seal::api {
	/**
	   Describes a pipeline.
	 */
	struct pipeline_description
	{
		enum
		{
			GraphicsPipeline,
			ComputationPipeline
		} type;
		
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
