#pragma once

#include <cstdlib>

#include "seal/opt/align_alloc.hpp"
#include "seal/types.hpp"
#include "seal/types/immovable.hpp"
#include "seal/types/rotating_queue.hpp"

namespace seal {
	/**
	   A non iterable inplace read-write storage that allocates large block of memory.
	   Gurantees the memory address of a value won't change implicitly.

	   \tparam PageSize: The size of each page.
	   \tparam StoreT: The storage unit to contain in the storage.
	 */
	template<size_t PageSize, typename StoreT>
	class paged_storage
	{
	public:

		using element_t = StoreT;

		explicit paged_storage() = default;

		paged_storage(paged_storage&&) noexcept;
		paged_storage& operator=(paged_storage&&) noexcept;

		~paged_storage();

		/**
		   Stores a value inside the storage.

		   \param value: The value to store.
		   \return: An immovable reference to the stored object.
		 */
		StoreT& store(StoreT value);

		/**
		   Erases an object from the storage.

		   \param value: The value to erase
		 */
		void erase(StoreT& value);

		/**
		   Disown a value from the storage, returning a removale reference to it.
		   Acts the same as erase but instead of destroying the value, moves it out the storage.

		   \param value: The value to disown.
		 */
		StoreT&& disown(StoreT& value);

	private:
		inline static constexpr u32 PAGE_MAGIC = 0xDEADBEEF;

#pragma pack(push, 1)

		struct secure_t
		{
			bool active = false;
			StoreT value = {};
		};

		inline static constexpr size_t PAGE_DATA_SIZE = std::bit_ceil(PageSize); // Round up the page
																			// size to a power of 2
		inline static constexpr size_t VALUES_IN_PAGE = PAGE_DATA_SIZE / sizeof(secure_t);

		union page_data_t
		{
			u8 bytes[PAGE_DATA_SIZE];
			secure_t storage[1];

			page_data_t()
				: bytes{0}
			{}

			~page_data_t() {}
		};

		struct page_t
		{
			u32 magic;

			aligned<page_t> next = nullptr;

			size_t used = 0;
			page_data_t data;

			page_t() = default;

			~page_t() {
				for (size_t i = 0; i < used; ++i) {
					if (data.storage[i].active) {
						std::destroy_at(&data.storage[i].value);
					}
				}
			}
		};

#pragma pack(pop)

		/**
		   Release the memory this storage owns.
		 */
		void release() noexcept;

		/**
		   Emplaces a new page in front of the current head.

		   \return: The newlt allocated head.
		 */
		void emplace_head();

		/**
		   Gets the secure_t of a StoreT.

		   \param value: The value to get the secure_t
		 */
		secure_t& to_secure_refrence(StoreT& value);

		/**
		   Finds an inactive secture value in the storage.
		 */
		secure_t& find_secure_value();

		/**
		   Allocates a new secure value in the paged storage.
		 */
		StoreT& allocate_secure_value();

		seal_no_copy(paged_storage);

		rotating_queue<std::uintptr_t> m_FreeSpots;
		aligned<page_t> m_Head = nullptr;
	};
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>::paged_storage(paged_storage&& other) noexcept
	: m_FreeSpots(std::move(other.m_FreeSpots))
	, m_Head(other.m_Head)
{
	other.m_Head = nullptr;
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>& seal::paged_storage<PagedSize, StoreT>::
operator=(paged_storage&& other) noexcept
{
	release();

	std::exchange(m_FreeSpots, other.m_FreeSpots);
	std::exchange(m_Head, other.m_Head);

	return *this;
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>::~paged_storage()
{
	release();
}

template<size_t PagedSize, typename StoreT>
StoreT& seal::paged_storage<PagedSize, StoreT>::store(StoreT value)
{
	auto& secure_value = allocate_secure_value();
	secure_value = std::move(value);

	return secure_value;
}

template<size_t PagedSize, typename StoreT>
void seal::paged_storage<PagedSize, StoreT>::erase(StoreT& value)
{
	// Disown the element to remove into this scope.
	auto&& disword_value = disown(value);

	// Destroy the disowned element ;)
	std::destroy_at(&disword_value);
}

template<size_t PagedSize, typename StoreT>
StoreT&& seal::paged_storage<PagedSize, StoreT>::disown(StoreT& value)
{
	// Locate the start of the page from the address.
	const auto page_begin = seal::align_down(reinterpret_cast<std::uintptr_t>(&value),
											 PAGE_DATA_SIZE);
	auto *page = reinterpret_cast<page_t *>(page_begin);

	// Ensure we actually have a page.
	seal_assert(PAGE_MAGIC == page->magic, "Value is not inside of a page!");

	// Get the secure_t of the value
	auto& secure = to_secure_refrence(value);
	seal_wassert(secure.active, "Attempting to disown an erased value!");

	// Mark the value as inactive.
	secure.active = false;

	// Register this empty spot.
	m_FreeSpots.enqueue(reinterpret_cast<std::uintptr_t>(&secure));

	// Move out the value inside the secure object.
	return std::move(secure.value);
}

template<size_t PagedSize, typename StoreT>
void seal::paged_storage<PagedSize, StoreT>::release() noexcept
{
	seal_mute_exceptions({});
}

template<size_t PagedSize, typename StoreT>
void seal::paged_storage<PagedSize, StoreT>::emplace_head()
{
	aligned<page_t> new_head = aligned_alloc<page_t>(PAGE_DATA_SIZE);
	new_head->magic = PAGE_MAGIC;
	new_head->next = std::move(m_Head);

	m_Head = std::move(new_head);
}

template<size_t PagedSize, typename StoreT>
StoreT& seal::paged_storage<PagedSize, StoreT>::allocate_secure_value()
{
	auto& secure_value = find_secure_value();

	seal_assert(!secure_value.active, "Attempting to allocate an active secure item");

	secure_value.active = true;
	return secure_value.value;
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>::secure_t& seal::paged_storage<PagedSize,
																	  StoreT>::find_secure_value()
{
	// If we know of an empty slot someware in the storage, use it.
	if(!m_FreeSpots.empty()) {
		auto free_spot = m_FreeSpots.peek();
		m_FreeSpots.dequeue();

		return *reinterpret_cast<secure_t *>(free_spot);
	}

	// If we are out values in the head page, allocate a new head page.
	// Note that we also know there are no empty slots left.
	if(!m_Head.is_valid() || m_Head->used >= VALUES_IN_PAGE) {
		emplace_head();
	}

	// Acquire the first empty secure_t in the head.
	return m_Head->data.storage[m_Head->used++];
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>::secure_t& seal::paged_storage<PagedSize, StoreT>::
	to_secure_refrence(StoreT& value)
{
	auto *secure_value_start = reinterpret_cast<u8 *>(&value) - offsetof(secure_t, value);

	return *reinterpret_cast<secure_t *>(secure_value_start);
}