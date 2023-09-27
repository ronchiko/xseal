#pragma once

#include <string>
#include <unordered_map>

#include "seal/api/back/uniform_buffer.hpp"
#include "seal/engine/render/pipeline/uniform_buffer.hpp"

namespace seal {
	namespace detail
	{
		inline std::unordered_map<std::string, uniform_buffer> g_UniformProviders = {};
	}

	using typeless_provider = char;

	template<typename BoundT>
	class pipeline_data_provider
	{
	public:
		/**
			Creates or opens a pipeline data provider by its name.
		 */
		static pipeline_data_provider create(const std::string& name)
		{
			if(detail::g_UniformProviders.contains(name)) {
				throw failure("A provider named {} already exists.");
			}

			detail::g_UniformProviders[name] = uniform_buffer::create<BoundT>();
			log::info("Initializing provider named {} of size {}", name, sizeof(BoundT));
			return open(name);
		}

		static pipeline_data_provider open(const std::string& name)
		{
			if(!detail::g_UniformProviders.contains(name)) {
				log::warning("Attempted to access provider named {}", name);
				throw failure("No provider named {}.");
			}

			return pipeline_data_provider(detail::g_UniformProviders[name]);
		}

		void update(const BoundT& data)
		{
			m_UniformBuffer->update(data);
		}

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
