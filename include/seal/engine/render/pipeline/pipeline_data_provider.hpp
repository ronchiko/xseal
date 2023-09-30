#pragma once

#include <string_view>
#include <unordered_map>

#include "seal/api/back/uniform_buffer.hpp"
#include "seal/engine/render/pipeline/uniform_buffer.hpp"

namespace seal {
	namespace detail
	{
		inline std::unordered_map<std::string_view, uniform_buffer> g_UniformProviders = {};
	}

	using typeless_provider = char;

	template<typename BoundT>
	class pipeline_data_provider
	{
	public:

		pipeline_data_provider() = default;

		/**
			Creates a pipeline data provider an assigns it a name.
			If the name is already occupied, throws an error. 

			\param name: The name of the provider.
		 */
		static pipeline_data_provider create(const std::string_view& name)
		{
			if(detail::g_UniformProviders.contains(name)) {
				throw failure("A provider named {} already exists.");
			}

			detail::g_UniformProviders[name] = uniform_buffer::create<BoundT>();
			log::info("Initializing provider named {} of size {}", name, sizeof(BoundT));
			return open(name);
		}

		/**
			Opens an existing provider using its name. If it does not exist throws an error.

			\param name: The name of the provider.
		 */
		static pipeline_data_provider open(const std::string_view& name)
		{
			if(!detail::g_UniformProviders.contains(name)) {
				log::warning("Attempted to access provider named {}", name);
				throw failure("No provider named {}.");
			}

			return pipeline_data_provider(detail::g_UniformProviders[name]);
		}

		/**
			Changes the data in the provider object.
		 */
		void update(const BoundT& data)
		{
			m_UniformBuffer->update(data);
		}

		/**
			Returns the abstracted api object this provider uses to data to pipelines.

			Use with api calls.
		 */
		[[nodiscard]] constexpr api::abstract_t abstracted() const
		{
			return m_UniformBuffer->abstracted();
		}

	private:
		explicit pipeline_data_provider(uniform_buffer& buffer)
			: m_UniformBuffer(&buffer)
		{
			seal_assert(nullptr != m_UniformBuffer, "Buffer is not allowed to be null.");
		}

		uniform_buffer *m_UniformBuffer = nullptr;
	};
}
