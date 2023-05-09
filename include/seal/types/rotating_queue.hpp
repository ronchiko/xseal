#pragma once

#include <vector>

#include "seal/debug.h"
#include "seal/panic.h"
#include "seal/types.hpp"

#include "seal/types/buffer.hpp"

namespace seal {
	template<typename T>
	class rotating_queue
	{
	public:
		rotating_queue() = default;

		/**
		   Enqueues an item into the queue.

		   \param item: The item to enqueue
		 */
		constexpr T& enqueue(T item);

		constexpr const T& peek() const;
		constexpr T& peek();

		/**
		   Removes the topmost element in the queue.

		   \note: If want to use it, acquire it with peek() beforehand.
		 */
		constexpr void dequeue();

		/**
		   Checks if the queue is empty.
		  
		   \return: True if the queue is empty.
		 */
		constexpr bool empty() const;

		/**
			Return the amount of elements in the queue
		 */
		constexpr size_t size() const;

	private:
		constexpr T& allocate_item();

		constexpr void reposition_buffer(seal::buffer<T>& buffer);

		constexpr size_t rotating_increment(size_t& value);

		static constexpr f32 GROWTH_RATE = 1.5f;

		seal::buffer<T> m_Storage;
		size_t m_Head = 0;
		size_t m_Tail = 0;
		size_t m_Count = 0;
	};
}

template<typename T>
constexpr T& seal::rotating_queue<T>::enqueue(T item)
{
	auto& obj = allocate_item();
	obj = std::move(item);

	return obj;
}

template<typename T>
constexpr const T& seal::rotating_queue<T>::peek() const
{
	seal_assert(!empty(), "Queue is empty!");

	return m_Storage[m_Head];
}

template<typename T>
constexpr T& seal::rotating_queue<T>::peek()
{
	seal_assert(!empty(), "Queue is empty!");

	return m_Storage[m_Head];
}

template<typename T>
constexpr void seal::rotating_queue<T>::dequeue()
{
	seal_assert(!empty(), "Queue is empty!");

	// If the queue is empty we cant dequeue.
	if(m_Count <= 0) {
		return;
	}

	--m_Count;
	rotating_increment(m_Head);
}

template<typename T>
constexpr bool seal::rotating_queue<T>::empty() const
{
	return 0 == m_Count;
}

template<typename T>
constexpr size_t seal::rotating_queue<T>::size() const
{
	return m_Count;
}

template<typename T>
constexpr T& seal::rotating_queue<T>::allocate_item()
{
	if(m_Storage.size() <= m_Count) {
		seal::buffer<T> new_storage(static_cast<size_t>(m_Count * GROWTH_RATE + 1));
		reposition_buffer(new_storage);

		m_Storage = std::move(new_storage);
		m_Head = 0;
		m_Tail = m_Count;
	}

	++m_Count;
	return m_Storage[rotating_increment(m_Tail)];
}

template<typename T>
constexpr void seal::rotating_queue<T>::reposition_buffer(seal::buffer<T>& target)
{
	if(m_Head >= m_Tail) {
		const auto target_midpoint = std::next(target.begin(), m_Tail);
		const auto queue_midpoint = std::next(m_Storage.begin(), m_Tail);

		// First copy from the head to the end of the storage.
		std::move(m_Storage.begin() + m_Head, m_Storage.end(), target_midpoint);
		// Then copy start of the queue to its tail.
		std::move(m_Storage.begin(), queue_midpoint, target.begin());
		return;
	}

	// Copy from head to tail
	std::move(m_Storage.begin() + m_Head, m_Storage.begin() + m_Tail, target.begin());
}

template<typename T>
constexpr size_t seal::rotating_queue<T>::rotating_increment(size_t& value)
{
	const size_t ret = value;
	value = (ret + 1) % m_Storage.size();
	return ret;
}
