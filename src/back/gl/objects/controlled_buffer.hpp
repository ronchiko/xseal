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
		result<void> load()
		{
			if(nullptr == m_Memory) {
				m_Memory = std::unique_ptr<ElementT[]>(new ElementT[m_Buffer.size_in<ElementT>()]);
			}
			return {};
		}

		/**
		   Frees the buffers memory (doesn't release the buffer)!.
		 */
		void unload()
		{
			if(nullptr == m_Memory) {
				return;
			}

			m_Memory = nullptr;
		}

		void write()
		{
			// Nothing to upload.
			if(nullptr == m_Memory) {
				seal::log::error("Attempted to upload a non allocated buffer");
				return;
			}

			// Rewrite the memory to the buffer.
			auto write_result = m_Buffer.write(0,
											   std::span{ m_Memory.get(),
														  m_Buffer.size_in<ElementT>() });
			if(!write_result) {
				seal::log::warning("Failed to write to gl buffer {}: {}", m_Buffer, write_result.error().what());
				return;
			}
		}

		result<std::span<ElementT>> read()
		{
			if(nullptr == m_Memory) {
				seal::log::error("Attempted to read into non allocated buffer");
				return seal::failure("Attempted to read into non allocated buffer");
			}

			seal_verify_result(m_Buffer.read_into<ElementT>(0,
															m_Buffer.size_in<ElementT>(),
															m_Memory.get()));
			return view();
		}

		constexpr buffer *operator->()
		{
			return &m_Buffer;
		}

		constexpr ElementT *get()
		{
			return m_Memory.get();
		}

		constexpr std::span<ElementT> view()
		{
			return { m_Memory.get(), m_Buffer.size_in<ElementT>() };
		}

		void flush() {
			// If we have something to flush, then flush it.
			if(nullptr != m_Memory) {
				write();
			}
		}

	private:
		buffer m_Buffer;
		std::unique_ptr<ElementT[]> m_Memory;
	};
}
