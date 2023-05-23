#pragma once

#include <nlohmann/json.hpp>

#include "seal/types/result.hpp"

#include "seal/api/resource.hpp"

namespace seal::json {
	/**
	   Parses a piece of data as a JSON.
	  
	   \param content: The content to parse a JSON.
	 */
	template<typename DataT>
	result<nlohmann::json> parse(const DataT& content)
	{
		try {
			return nlohmann::json::parse(content);
		} catch(const std::exception& err) {
			seal::log::error("Failed to parse JSON: {}", err.what());
			return seal::fail<seal::failure::fail_type::ExternalFailure>(err.what());
		}
	}

	/**
	   Loads a resource as a JSON object.
	  
	   \param resource: The resource to load as a JSON.
	 */
	inline result<nlohmann::json> load_as_json(resource& resource)
	{
		auto content = resource->load_all();
		seal_verify_result(content);

		return parse(*content);
	}
}
