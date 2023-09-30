#pragma once

#include "seal/types.hpp"
#include "seal/api/back/uniform_buffer.hpp"

namespace seal
{
	class pipeline_stage;

	class uniform_buffer
	{
	public:
		uniform_buffer() = default;

		/**
			Creates a new uniform buffer from a set of data.
		 */
		template<typename ResourceT>
		static uniform_buffer create();

		/**
			Updates the data inside the uniform_buffer.

			\param data: The data to initialize the buffer with.
		 */
		template<typename ResourceT>
		void update(const ResourceT& data) const;

		[[nodiscard]] constexpr api::abstract_t abstracted() const;

	private:
		explicit uniform_buffer(api::abstract_t uniform_buffer);

		api::abstract_t m_UniformBuffer = api::UNTAGGED_OBJECT;
	};
}


template <typename ResourceT>
seal::uniform_buffer seal::uniform_buffer::create()
{
	auto ubo = api::allocate_uniform_buffer(sizeof(ResourceT));

	return uniform_buffer(std::move(ubo));
}

template<typename ResourceT>
void seal::uniform_buffer::update(const ResourceT& data) const
{
	api::update_uniform_buffer(m_UniformBuffer, reinterpret_cast<const void *>(&data));
}

constexpr seal::api::abstract_t seal::uniform_buffer::abstracted() const
{
	return m_UniformBuffer;
}

