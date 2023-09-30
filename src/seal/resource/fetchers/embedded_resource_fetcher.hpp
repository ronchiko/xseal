#pragma once

#include <unordered_map>

#include "../resource_fetcher.hpp"

namespace seal {
	/**
		Resource fetcher for embedded resources.
	 */
	class embedded_resource_fetcher final : public resource_fetcher
	{
	public:
		explicit embedded_resource_fetcher();

		/**
		   Fetches an embedded resource using its path.
		  
		   \param path: The path to the resource
		 */
		virtual resource_interface* fetch(const std::string& path) override;

		/**
		   Gets the global resource fetcher.
		 */
		static embedded_resource_fetcher *get();

		/**
		   Registers a resource as an embedded resource.
		  
		   \param path: The path to the resource
		   \param data: The content of the resource
		 */
		void register_resource(std::string path, std::span<const u8> data);

	private:
		struct embedded_resource_info
		{
			const void *data;
			size_t size;
		};

		inline static std::unique_ptr<embedded_resource_fetcher> s_Instance = nullptr;
	
		std::unordered_map<std::string, embedded_resource_info> m_EmbeddedResources;
	};
}
