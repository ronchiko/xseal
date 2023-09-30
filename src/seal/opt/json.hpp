#pragma once

#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
#endif
#include <nlohmann/json.hpp>

#ifdef __clang__
#pragma clang diagnostic pop
#endif

#include "seal/types/result.hpp"
#include "seal/types/conversion_table.hpp"

#include "seal/api/resource.hpp"

namespace seal::json {
	/**
	   Parses a piece of data as a JSON.
	  
	   \param content: The content to parse a JSON.
	 */
	template<typename DataT>
	nlohmann::json parse(const DataT& content)
	{
		try {
			return nlohmann::json::parse(content);
		} catch(const std::exception& err) {
			seal::log::error("Failed to parse JSON: {}", err.what());
			throw seal::fail<seal::failure::fail_type::ExternalFailure>(err.what());
		}
	}

	/**
	   Loads a resource as a JSON object.
	  
	   \param resource: The resource to load as a JSON.
	 */
	inline nlohmann::json load_as_json(const resource& resource)
	{
		const auto content = resource->load_all();
		return parse(content);
	}

	template<typename EnumT>
	inline EnumT parse_to_enum(const nlohmann::json& json)
	{
		if (!json.is_string()) {
			throw seal::failure("Enum expected to be a string.");
		}

		return convert<EnumT>(json.get<std::string>());
	}
}
