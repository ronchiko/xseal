#pragma once

#include "seal/defenitions.h"
#include "seal/panic.h"

namespace seal {

	void *aligned_alloc(size_t alignment, size_t size);
	void aligned_free(void *address);

	/**
	   Allocates aligned memory.
	  
	   \param alignment: The alignment of the memory.
	   \param arguments: Constructor arguments for the type.
	 
		\note: Must free with aligned_free
	 */
	template<typename AllocT, typename... ArgumentsT>
	AllocT *aligned_alloc(const size_t alignment, ArgumentsT&&... arguments)
	{
		auto* aligned_block = static_cast<AllocT *>(aligned_alloc(alignment, sizeof(AllocT)));
		std::construct_at(aligned_block, arguments...);

		return aligned_block;
	}

	/**
	   An object whose address is aligned to some value.

	   \note: This object is needed because aligned allocation dont always follow
			  the classic free/delete object.
	 */
	template<typename ValueT>
	struct aligned
	{
	public:
		constexpr aligned();
		constexpr aligned(ValueT *ptr);

		aligned(const aligned&) = delete;
		aligned& operator=(const aligned&) = delete;

		constexpr aligned(aligned&&) noexcept;
		aligned& operator=(aligned&&) noexcept;

		~aligned();

		constexpr ValueT& operator*() noexcept;
		constexpr const ValueT& operator*() const noexcept;

		constexpr ValueT *operator->() noexcept;
		constexpr const ValueT *operator->() const noexcept;

		/**
		   Checks if the value inside the object is valid.
		 */
		constexpr bool is_valid() const noexcept;

	private:
		void release() noexcept;

		ValueT *m_Value = nullptr;
	};
}

template<typename AllocT>
constexpr seal::aligned<AllocT>::aligned()
	: aligned(nullptr)
{}

template<typename AllocT>
constexpr seal::aligned<AllocT>::aligned(AllocT* ptr)
	: m_Value(ptr)
{}

template<typename AllocT>
constexpr seal::aligned<AllocT>::aligned(aligned&& other) noexcept
	: m_Value(other.m_Value)
{
	other.m_Value = nullptr;
}

template<typename AllocT>
seal::aligned<AllocT>& seal::aligned<AllocT>::operator=(aligned&& other) noexcept
{
	release();

	m_Value = other.m_Value;
	other.m_Value = nullptr;

	return *this;
}

template<typename AllocT>
seal::aligned<AllocT>::aligned::~aligned()
{
	release();
}

template<typename AllocT>
constexpr AllocT& seal::aligned<AllocT>::operator*() noexcept
{
	return *m_Value;
}

template<typename AllocT>
constexpr const AllocT& seal::aligned<AllocT>::operator*() const noexcept
{
	return *m_Value;
}

template<typename AllocT>
constexpr AllocT* seal::aligned<AllocT>::operator->() noexcept
{
	return m_Value;
}

template<typename AllocT>
constexpr const AllocT*seal::aligned<AllocT>::operator->() const noexcept
{
	return m_Value;
}

template<typename AllocT>
constexpr bool seal::aligned<AllocT>::is_valid() const noexcept
{
	return nullptr != m_Value;
}

template<typename AllocT>
void seal::aligned<AllocT>::release() noexcept
{
	seal_mute_exceptions({
		if(nullptr != m_Value) {
			// Destroy the value we have.
			std::destroy_at(m_Value);
			// Deallocate the memory
			seal::aligned_free(m_Value);
			m_Value = nullptr;
		}
	});
}