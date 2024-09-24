#pragma once

#include "seal/types.hpp"

namespace seal::ranges {
	template<typename IteratorT, typename SentinelT>
	class enumerator_view
	{
	public:
		struct enumerator_sentinel
		{};

		using iterator_type = IteratorT;
		using sentinel_type = SentinelT;
		using value_type = decltype(*std::declval<iterator_type>());

		constexpr explicit enumerator_view(iterator_type iterator, sentinel_type end)
			: m_Index(0)
			, m_Iterator(std::move(iterator))
			, m_End(std::move(end))
		{}

		[[nodiscard]] constexpr std::pair<u32, value_type> operator*() const
		{
			return std::make_pair(m_Index, *m_Iterator);
		}

		[[nodiscard]] constexpr enumerator_view& operator++()
		{
			++m_Index;
			++m_Iterator;

			return *this;
		}

		[[nodiscard]] constexpr bool operator==(const enumerator_view& e) const
		{
			return e.m_Iterator == m_Iterator;
		}

		[[nodiscard]] constexpr bool operator==(const enumerator_sentinel& e) const
		{
			return m_End == m_Iterator;
		}

		template<typename T>
		[[nodiscard]] constexpr bool operator!=(const T& e) const
		{
			return !operator==(e);
		}

		constexpr enumerator_view& begin()
		{
			return *this;
		}

		[[nodiscard]] constexpr const enumerator_view& begin() const
		{
			return *this;
		}

		[[nodiscard]] static constexpr enumerator_sentinel end()
		{
			return enumerator_sentinel{};
		}

	private:
		u32 m_Index;
		iterator_type m_Iterator;
		sentinel_type m_End;
	};

	namespace detail {
		struct lazy_enumerator_constructor
		{};
	}

	inline detail::lazy_enumerator_constructor enumerate()
	{
		return detail::lazy_enumerator_constructor{};
	}
}

template<typename ContainerT>
auto operator|(const ContainerT& container, seal::ranges::detail::lazy_enumerator_constructor)
{
	return seal::ranges::enumerator_view(container.begin(), container.end());
}
