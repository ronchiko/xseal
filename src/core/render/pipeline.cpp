#include "seal/render/pipeline.hpp"

#include "seal/api/back/pipeline.hpp"

namespace seal {

	pipeline& pipeline::operator=(pipeline&& other) noexcept
	{
		release();

		m_Id = other.m_Id;
		other.m_Id = INVALID_ID;

		return *this;
	}

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

		return pipeline{ new_pipeline };
	}

	void pipeline::release() noexcept
	{
		seal_mute_exceptions({ seal::api::free_pipeline(m_Id); });
	}
}
