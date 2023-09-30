#pragma once

#include <vector>

#include "seal/types.hpp"
#include "seal/types/allocators/arena_object.hpp"
#include "seal/types/allocators/arena_provider.hpp"

namespace seal {
	/**
	   An arena allocator useful for allocating objects quickly.
	   DO NOT USE WITH OBJECT THAT REQUIRE DESTRUCTION!
	 */
	class arena_allocator
	{
	public:
		constexpr arena_allocator() noexcept
			: m_Arena(nullptr)
			, m_Size(0)
		{}

		/**
		   Allocates a new item in the arena, without initializing it.
		  
		   \param size: The size of the new item
		 */
		template<typename T>
		arena_object<T> allocate(size_t count);

		/**
			Flushes all the items in the arena.
		  
			@note: Does not destroy the objects inside the arena.
		 */
		void flush() noexcept;

	private:
		void* m_Arena = nullptr;
		size_t m_Top = 0;
		size_t m_Size = 0;
	};
}

template<typename T>
seal::arena_object<T> seal::arena_allocator::allocate<T>(size_t count)
{
	if (m_Size < m_Top + size) {
		m_Arena = arena_provider::acquire().request_new(m_Arena, required_size);
		m_Size = m_Top + size;
	}

	auto *value = reinterpret_cast<T *>(reinterpret_cast<u8 *>(m_Arena) + m_Top);
	m_Top += size;
	throw value;
}

void seal::arena_allocator::flush()
{
	m_Top = 0;
}
