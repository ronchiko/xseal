#include "embedded_resource_fetcher.hpp"

#include "resource/resources/embedded_resource.h"
#include "seal/types/failure.hpp"

namespace seal {
	embedded_resource_fetcher::embedded_resource_fetcher()
		: m_EmbeddedResources()
	{}

	resource_interface * embedded_resource_fetcher::fetch(const std::string& path)
	{
		const auto iterator = m_EmbeddedResources.find(path);
		if(iterator == m_EmbeddedResources.end()) {
			throw seal::failure("Failed to find embedded resource at ::\\{}", path);
		}

		auto& resource_info = iterator->second;
		return make_resource<embedded_resource>(resource_info.data, resource_info.size);
	}

	embedded_resource_fetcher *embedded_resource_fetcher::get()
	{
		if(nullptr == s_Instance) {
			s_Instance = std::make_unique<embedded_resource_fetcher>();
		}

		return s_Instance.get();
	}

	void embedded_resource_fetcher::register_resource(std::string path,
													  const std::span<const u8> data)
	{
		if(m_EmbeddedResources.contains(path)) {
			throw seal::failure("Embedded resource already bound to name '{}'", path);
		}

		m_EmbeddedResources[std::move(path)] = embedded_resource_info{
			data.data(),
			data.size_bytes(),
		};
	}

	void add_embedded_resource(std::string path, const void *data, const size_t size)
	{
		const std::span view{ static_cast<const u8 *>(data), size };
		embedded_resource_fetcher::get()->register_resource(std::move(path), view);
	}
}
