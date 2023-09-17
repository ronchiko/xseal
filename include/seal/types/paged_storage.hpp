#pragma once

#include <cstdlib>
#include <unordered_map>

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
		paged_storage() = default;

		paged_storage(paged_storage&&) = delete;
		paged_storage& operator=(paged_storage&&) = delete;

		paged_storage(const paged_storage&) = delete;
		paged_storage& operator=(const paged_storage&) = delete;

		~paged_storage();

		/**
		   Stores a value inside the storage.

		   \param value: The value to store.
		   \return: An immovable reference to the stored object.
		 */
		StoreT& store(StoreT value);

		template<typename... ArgTs>
		StoreT& emplace(ArgTs&&...arguments)
		{
			return store(StoreT(std::forward<ArgTs>(arguments)...));
		}

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
		StoreT disown(StoreT& value);

	private:
		struct node_t
		{
			node_t *previous = nullptr;
			node_t *next = nullptr;
			StoreT element;
		};

		/**
		   Gets a node from a value.

		   \param value: The value to retreive to node for.
		 */
		node_t *retreive_node(void *value);

		node_t *m_Head = nullptr;
	};
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>::~paged_storage()
{
	while (nullptr != m_Head) {
		auto *next = m_Head->next;
		delete m_Head;
		m_Head = next;
	}
}

template<size_t PagedSize, typename StoreT>
StoreT& seal::paged_storage<PagedSize, StoreT>::store(StoreT value)
{
	auto new_head = new node_t{ nullptr, nullptr, std::move(value) };
	if(nullptr != m_Head) {
		m_Head->previous = new_head;
	}

	new_head->next = m_Head;
	m_Head = new_head;

	return m_Head->element;
}

template<size_t PagedSize, typename StoreT>
void seal::paged_storage<PagedSize, StoreT>::erase(StoreT& value)
{
	// Disown the element to remove into this scope.
	auto disword_value = disown(value);
}

template<size_t PagedSize, typename StoreT>
StoreT seal::paged_storage<PagedSize, StoreT>::disown(StoreT& value)
{
	auto *node_pointer = retreive_node(&value);
	node_t *node = nullptr;

	// We take the ownership from the node that points to us.
	if(nullptr != node_pointer->previous) {
		if(node = node_pointer->previous->next) {
			node->previous->next = node->next;
		}
	} else {
		node = m_Head;
		m_Head = node->next;
	}

	if(nullptr != node->next) {
		node->next->previous = node->previous;
	}

	auto owned_value = std::move(node->element);
	delete node;

	return owned_value;
}

template<size_t PagedSize, typename StoreT>
seal::paged_storage<PagedSize, StoreT>::node_t *seal::paged_storage<PagedSize,
																	StoreT>::retreive_node(void *value)
{
	auto *pointer = static_cast<u8 *>(value);
	pointer -= offsetof(node_t, element);

	return reinterpret_cast<node_t *>(pointer);
}
