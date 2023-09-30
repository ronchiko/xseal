#pragma once

#include <memory>
#include <span>

#include "seal/opt/math.hpp"

namespace seal {
	template<typename T>
	class buffer
	{
	public:
		using element_t = T;

		constexpr buffer() noexcept
			: m_Container()
			, m_Size(0)
		{}

		constexpr buffer(nullptr_t) noexcept
			: buffer()
		{}

		buffer& operator=(nullptr_t) noexcept {
			m_Container.release();
			m_Size = 0;

			return *this;
		}

		constexpr explicit buffer(size_t count);

		constexpr buffer(const buffer&);
		constexpr buffer(buffer&&) noexcept;

		buffer& operator=(const buffer&);
		constexpr buffer& operator=(buffer&&) noexcept;

		~buffer() noexcept = default;

		/**
		   The size of the buffer (in T's).

		   \return
		 */
		constexpr size_t size() const;

		/**
			The data inside the buffer
		 */
		constexpr T *data();
		constexpr const T *data() const;

		/**
		   Resizes the buffer.

		   \param new_size: The new size of the buffer.
		 */
		constexpr void resize(size_t new_size);

		/**
		   Access an element at a position in the buffer.

		   \param index: The index inside the buffer.

		   \note: This access is unchecked!! make sure you know you are accessing valid locations.
		 */
		constexpr T& operator[](size_t index);
		constexpr const T& operator[](size_t index) const;

		/**
		   Views this buffer as a diffrent type.

		   If the buffer's size doesn't align to the type's size, align downwards.
		 */
		template<typename ViewT>
		constexpr std::span<ViewT> view_as();
		template<typename ViewT>
		constexpr const std::span<ViewT> view_as() const;

		constexpr T *begin();
		constexpr const T *begin() const;

		constexpr T *end();
		constexpr const T *end() const;

	private:
		std::unique_ptr<T[]> m_Container;
		size_t m_Size;
	};
}

template<typename T>
constexpr seal::buffer<T>::buffer(size_t size)
	: m_Container(new T[size]{})
	, m_Size(size)
{}

template<typename T>
constexpr seal::buffer<T>::buffer(const buffer& other)
	: m_Container(new T[other.m_Size])
	, m_Size(m_Size)
{
	std::copy(other.begin(), other.end(), m_Container.get());
}

template<typename T>
constexpr seal::buffer<T>::buffer(buffer&& other) noexcept
	: m_Container(std::move(other.m_Container))
	, m_Size(other.m_Size)
{
	other.m_Container = nullptr;
	other.m_Size = 0;
}

template<typename T>
seal::buffer<T>& seal::buffer<T>::operator=(const buffer& other)
{
	m_Container = new T[m_Size = other.m_Size];
	std::copy(other.begin(), other.end(), m_Container.get());

	return *this;
}

template<typename T>
constexpr seal::buffer<T>& seal::buffer<T>::operator=(buffer&& other) noexcept
{
	std::swap(m_Container, other.m_Container);
	std::exchange(m_Size, other.m_Size);

	return *this;
}

template<typename T>
constexpr size_t seal::buffer<T>::size() const
{
	return m_Size;
}

template<typename T>
constexpr T *seal::buffer<T>::data()
{
	return m_Container.get();
}

template<typename T>
constexpr const T *seal::buffer<T>::data() const
{
	return m_Container.get();
}

template<typename T>
constexpr void seal::buffer<T>::resize(const size_t size)
{
	// Dont do anything if the resize is a shrink or the same size.
	if(size <= m_Size) {
		m_Size = size;
		return;
	}
	
	// If the resize is a growth then we need to allocate a new buffer and copy the elements.
	std::unique_ptr<T[]> new_buffer(new T[size]); 
	std::move(begin(), end(), new_buffer.get());
	m_Container = std::move(new_buffer);
	m_Size = size;
}

template<typename T>
constexpr T& seal::buffer<T>::operator[](size_t index)
{
	return m_Container.get()[index];
}

template<typename T>
constexpr const T& seal::buffer<T>::operator[](size_t index) const
{
	return m_Container.get()[index];
}


template<typename T>
template<typename ViewT>
constexpr std::span<ViewT> seal::buffer<T>::view_as()
{
	auto *view = reinterpret_cast<ViewT *>(m_Container.get());
	return std::span<ViewT>(view, seal::align_down(m_Size * sizeof(T), sizeof(ViewT)));
}

template<typename T>
template<typename ViewT>
constexpr const std::span<ViewT> seal::buffer<T>::view_as() const
{
	auto *view = reinterpret_cast<ViewT *>(m_Container.get());
	return std::span<ViewT>(view, seal::align_down(m_Size * sizeof(T), sizeof(ViewT)));
}

template<typename T>
constexpr T* seal::buffer<T>::begin()
{
	return m_Container.get();
}

template<typename T>
constexpr const T *seal::buffer<T>::begin() const
{
	return m_Container.get();
}

template<typename T>
constexpr T *seal::buffer<T>::end()
{
	return m_Container.get() + m_Size;
}

template<typename T>
constexpr const T *seal::buffer<T>::end() const
{
	return m_Container.get() + m_Size;
}