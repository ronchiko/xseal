#include "seal/engine/render/pipeline/pipeline_stage.hpp"

#include "seal/defenitions.h"
#include "seal/api/back/pipeline.hpp"
#include "seal/types/error_value.hpp"

using namespace seal::api;

namespace seal {

	pipeline_stage::~pipeline_stage() noexcept
	{
		release();
	}

	void pipeline_stage::bind() const
	{
		seal::api::bind_pipeline_stage(m_Id);
	}

	api::uniform_query_information pipeline_stage::query_uniform(const std::string& name) const
	{
		return api::query_pipeline_uniform(m_Id, name);
	}

	void pipeline_stage::update_uniform(const u32 location, const u32 value) const
	{
		api::update_pipeline_uniform(m_Id, uniform_update_options{ location, value });
	}

	bool pipeline_stage::operator==(const pipeline_stage& other) const
	{
		return other.m_Id == m_Id;
	}

	bool pipeline_stage::operator!=(const pipeline_stage& other) const
	{
		return other.m_Id != m_Id;
	}

	void pipeline_stage::release() noexcept
	{
		if(seal_is_error(m_Id)) {
			return;
		}

		seal_mute_exceptions({ seal::api::free_pipeline_stage(m_Id); });
		m_Id = seal::error_value_v<api::abstract_t>;
	}
}
