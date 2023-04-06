#include "embedded_resource_fetcher.hpp"

#include "resource/resources/embedded_resource.h"

namespace seal {
	embedded_resource_fetcher::embedded_resource_fetcher()
		: m_EmbeddedResources()
	{}

	result<resource_interface *> embedded_resource_fetcher::fetch(const std::string& path)
	{
		auto rsrc_iter = m_EmbeddedResources.find(path);
		if(rsrc_iter == m_EmbeddedResources.end()) {
			return seal::failure("Failed to find embedded resource at ::\\{}", path);
		}

		auto& resource_info = (*rsrc_iter).second;
		return make_resource<embedded_resource>(resource_info.m_Data, resource_info.m_Size);
	}

	embedded_resource_fetcher *embedded_resource_fetcher::get()
	{
		if(nullptr == g_Instance) {
			g_Instance = std::make_unique<embedded_resource_fetcher>();
		}

		return g_Instance.get();
	}

	result<void> embedded_resource_fetcher::register_resource(std::string path,
															  std::span<const u8> data)
	{
		if(m_EmbeddedResources.find(path) != m_EmbeddedResources.end()) {
			return seal::failure("Embedded resource already bound to name '{}'", path);
		}

		m_EmbeddedResources[std::move(path)] = embedded_resource_info{
			data.data(),
			data.size_bytes(),
		};

		return {};
	}

	result<void> add_embedded_resource(std::string path, const void *data, size_t size)
	{
		return embedded_resource_fetcher::get()
			->register_resource(std::move(path),
								std::span{ reinterpret_cast<const u8 *>(data), size });
	}
}
