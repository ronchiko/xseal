#pragma once

#include <memory>
#include <span>

#include "objects/buffer.hpp"

namespace seal::gl {
	template<typename ElementT>
	class controlled_buffer
	{
	public:
		constexpr explicit controlled_buffer() = default;

		explicit controlled_buffer(buffer buffer)
			: m_Buffer(std::move(buffer))
			, m_Memory(nullptr)
		{}

		/**
		   Load the buffers content into memory.
		 */
		result<std::span<ElementT>> load()
		{
			if(nullptr != m_Memory) {
				return std::span{ m_Memory.get(), m_Buffer.size() };
			}

			auto read_result = m_Buffer.read<ElementT>(0, m_Buffer.size());
			seal_verify_result(read_result);

			m_Memory = std::move(*read_result);
			return std::span{ m_Memory.get(), m_Buffer.size() };
		}

		/**
		   Frees the buffers memory (doesn't release the buffer)!.
		 */
		void unload()
		{
			if(nullptr == m_Memory) {
				return;
			}

			// Rewrite the memory to the buffer.
			auto write_result = m_Buffer.write(0, std::span{ m_Memory.get(), m_Buffer.size() });
			if(!write_result) {
				seal::log::warning("Failed to write to gl buffer: {}", failure_message(last_err()));
				return;
			}

			m_Memory = nullptr;
		}

		constexpr buffer *operator->()
		{
			return &m_Buffer;
		}

		constexpr ElementT *get()
		{
			return m_Memory.get();
		}

	private:
		buffer m_Buffer;
		std::unique_ptr<ElementT[]> m_Memory;
	};
}
