#pragma once

#include <vector>

#include "seal/engine/render/pipeline/pipeline_stage.hpp"
#include "seal/engine/resource.hpp"

namespace seal {

	class pipeline
	{
	public:
		explicit pipeline(api::pipeline_stage_type type,
						  std::vector<pipeline_stage> stages);

		/**
			The amount of stages in a pipeline.
		 */
		[[nodiscard]] constexpr u32 count_stages() const
		{
			return static_cast<u32>(m_Stages.size());
		}

		constexpr pipeline_stage& at(const u32 index)
		{
			return m_Stages[index];
		}

	private:
		api::pipeline_stage_type m_Type;
		std::vector<pipeline_stage> m_Stages;
	};
}

template<>
struct seal::resource_loader<seal::pipeline>
{
public:
	static std::shared_ptr<seal::pipeline> load(const resource& resource);
};
