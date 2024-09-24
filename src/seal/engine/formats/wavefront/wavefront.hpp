#pragma once

#include "seal/engine/render/pipeline/pipeline.hpp"
#include "seal/api/resource.hpp"

namespace seal
{



	class wavefront_object
	{
		
	};

	wavefront_object load_wavefront(const resource& resource);
}

template<>
struct seal::resource_loader<seal::wavefront_object>
{
public:
	[[nodiscard]] static std::shared_ptr<wavefront_object> load(resource& resource)
	{
		return std::make_shared<wavefront_object>(load_wavefront(resource));
	}
};