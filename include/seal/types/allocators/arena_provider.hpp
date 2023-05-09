#pragma once

#include <memory>
#include <vector>

#include "seal/opt/math.hpp"

namespace seal {
	class arena_provider
	{
	public:
		constexpr arena_provider() noexcept
			: m_Lists()
		{
			for(auto& list : m_Lists) {
				list = nullptr;
			}
		}

		/**
		   Gets the current arena provider.
		 */
		inline static arena_provider& acquire();

		/**
		   Requests a new arena.

		   \param previous: The previously acquired arena (nullptr for the first time)
		   \param requested_size: The size of the arena.
		   \return: The arena
		 */
		inline void *request_new(void *previous_arena, size_t requested_size);

		/**
		   Dismisses an arena, marking it as unused.
		  
		   \param arena: The arena to free.
		 */
		inline void dismiss(void *arena);

	private:
		constexpr static size_t ARENA_SIZE_ALIGNMENT = 1 << 12;
		constexpr static size_t ARENA_LIST_COUNT = 8;

		constexpr static size_t HEADER_USED_BIT = seal::bit(0);
		constexpr static size_t HEADER_SIZE_MASK = ~(ARENA_SIZE_ALIGNMENT - 1);

		static inline std::unique_ptr<arena_provider> g_Provider = {};

		struct arena_header
		{
			arena_header *next;
			arena_header *prev;

			size_t size_and_flags;

			constexpr bool is_used() const noexcept
			{
				return size_and_flags & HEADER_USED_BIT;
			}

			/**
			   The usable size of that arena.
			 */
			constexpr size_t usable_size() const noexcept
			{
				return (size_and_flags & HEADER_SIZE_MASK) - sizeof(arena_header);
			}

			void mark_as_unused() noexcept
			{
				size_and_flags &= ~HEADER_USED_BIT;
			}

			void mark_as_used() noexcept
			{
				size_and_flags |= HEADER_USED_BIT;
			}
		};

		/**
		   Allocates a new header without binding it to a list.

		   \param size: The usable size of the header
		 */
		arena_header *allocate_header(size_t size);
		/**
		   Finds a suitable header in the list.

		   \param head: The head of the list
		   \param size: A minimal usable size that would be suitable
		   \return: nullptr if no suitable header was found or the located header
		 */
		arena_header *find_suiting_header(arena_header *head, size_t size);

		/**
		   Gets the usable part of a header.
		 */
		void *arena_usable_part(arena_header *header) noexcept;

		arena_header *m_Lists[ARENA_LIST_COUNT] = {};
	};
}

seal::arena_provider& seal::arena_provider::acquire()
{
	if(nullptr == g_Provider) {
		g_Provider = std::make_unique<arena_provider>();
	}

	return *g_Provider;
}

void *seal::arena_provider::request_new(void *previous, size_t size)
{
	const auto aligned_size = seal::align(size, ARENA_SIZE_ALIGNMENT);
	const auto list_index = seal::clamp((aligned_size >> 12) - 1, 0llu, ARENA_LIST_COUNT - 1);

	if(nullptr != previous) {
		auto *previous_header = reinterpret_cast<arena_header *>(reinterpret_cast<u8 *>(previous) -
																 sizeof(arena_header));

		// The previous header has enough space, then reuse it.
		if(previous_header->usable_size() >= size) {
			return previous;
		}

		// The previous arena isn't enought, we need to allocate a new one
		dismiss(previous);
	}

	auto& head = m_Lists[list_index];
	// No allocated arenas for a size limit.
	if(nullptr == head) {
		head = allocate_header(size);
		return arena_usable_part(head);
	}

	auto target_header = find_suiting_header(head, size);
	// Didnt find any empty arenas, allocate a new one
	if(nullptr == target_header) {
		target_header = allocate_header(size);
		// Add the header as the last one in its list.
		target_header->next = head;
		(target_header->prev = head->prev)->next = target_header;
		head->prev = target_header;
	}
	// We found or allocated a suitable header :)
	target_header->mark_as_used();
	return arena_usable_part(target_header);
}

void seal::arena_provider::dismiss(void *arena) noexcept
{
	// No arena
	if(nullptr == arena) {
		return;
	}

	// Marks the arena as unused.
	auto *previous_header = reinterpret_cast<arena_header *>(reinterpret_cast<u8 *>(arena) -
															 sizeof(arena_header));
	previous_header->mark_as_unused();
	const auto aligned_size = align(previous_header->usable_size(), ARENA_SIZE_ALIGNMENT);
	const auto list_index = seal::clamp((aligned_size >> 12) - 1, 0llu, ARENA_LIST_COUNT - 1);
	m_Lists[list_index] = previous_header;
}

seal::arena_provider::arena_header *seal::arena_provider::allocate_header(size_t size)
{
	const auto full_size = seal::align(size + sizeof(arena_header), ARENA_SIZE_ALIGNMENT);
	auto *buffer = new u8[full_size];
	auto *header = reinterpret_cast<arena_header *>(buffer);

	// Set the header values
	header->next = header->prev = nullptr;
	header->size_and_flags = full_size;
	header->mark_as_used();

	return header;
}

seal::arena_provider::arena_header *seal::arena_provider::find_suiting_header(arena_header *head,
																			  size_t size)
{
	// No head was supplied, the size is empty
	if(nullptr == head || 0 == size) {
		return nullptr;
	}

	const auto *start = head;
	do {
		// If the arena is not used and has enough space, return it
		if(!head->is_used() && head->usable_size() >= size) {
			return head;
		}

		head = head->next;
	} while(start != head); // End the search when we reached the start.

	// Nothing suitable was found.
	return nullptr;
}

void *seal::arena_provider::arena_usable_part(arena_header *header) noexcept {
	return reinterpret_cast<u8 *>(header) + sizeof(arena_header);
}
