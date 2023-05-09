#include "seal/render/pipeline.hpp"

#include "seal/api/back/pipeline.hpp"
#include "seal/types/error_value.hpp"

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

	void pipeline::release() noexcept
	{
		if(seal_is_error(m_Id)) {
			return;
		}

		seal_mute_exceptions({ seal::api::free_pipeline(m_Id); });
	}
}
