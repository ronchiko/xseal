#include "seal/api/resource.hpp"

#include "resource/fetchers/embedded_resource_fetcher.hpp"
#include "resource/resource_fetcher.hpp"

namespace seal {
	using fetch_request = std::pair<resource_fetcher *, std::string>;

	/**
	   Parses a resource path and returns the fetcher that is related to it, and the path to fetch
	   for.

	   \param path: The path to parse
	 */
	static result<fetch_request> parse_resource_path(const std::string& path)
	{
		const auto offset = path.find("::\\");
		auto suffix = path.substr(offset + 3);
		if(0 == offset) {
			return fetch_request(embedded_resource_fetcher::get(), std::move(suffix));
		}

		return seal::failure("Failed to parse resource path '{}'", path);
	}

	result<resource> load_resource(const std::string& path)
	{
		auto parse_result = parse_resource_path(path);
		seal_verify_result(parse_result);

		auto [fetcher, sub_path] = *parse_result;
		auto resource = fetcher->fetch(sub_path);
		seal_verify_result(resource);

		return std::make_shared<unique_resource>(path, *resource);
	}
}
