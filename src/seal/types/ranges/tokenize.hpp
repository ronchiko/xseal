#pragma once

#include <iterator>
#include <string>

namespace seal::ranges {
	namespace detail {
		template<typename CharT>
		class tokenizer_view
		{
		public:
			struct sentinel
			{};

			using char_type = CharT;
			using string_type = std::basic_string<char_type>;
			using view_type = std::basic_string_view<char_type>;

			using iterator_type = typename view_type::const_iterator;

			explicit tokenizer_view(const view_type& text, char_type value)
				: m_Delimiter(value)
				, m_Viewed(text)
				, m_Current(m_Viewed.begin())
				, m_Previous(m_Viewed.begin())
			{
				operator++();
			}

			constexpr auto& operator++()
			{
				do {
					m_Previous = m_Current;
					if(m_Viewed.end() == m_Current)
					{
						break;
					}

					m_Current = std::find(m_Current + 1, m_Viewed.end(), m_Delimiter);
				} while(m_Previous == m_Current && m_Viewed.end() != m_Current);

				return *this;
			}

			constexpr view_type operator*() const
			{
				auto offset = std::distance(m_Viewed.begin(), m_Previous);
				const auto size = std::distance(m_Previous, m_Current);

				if(offset != 0) {
					++offset;
					return m_Viewed.substr(offset, size - 1);
				}

				return m_Viewed.substr(0, size);
			}

			constexpr tokenizer_view& begin()
			{
				return *this;
			}

			[[nodiscard]] constexpr const tokenizer_view& begin() const
			{
				return *this;
			}

			constexpr static auto end() 
			{
				return sentinel{};
			}

			constexpr bool operator==(const sentinel) const
			{
				return m_Viewed.end() == m_Current && m_Viewed.end() == m_Previous;
			}

			constexpr bool operator!=(const sentinel o) const
			{
				return !operator==(o);
			}

		private:
			char_type m_Delimiter;
			view_type m_Viewed;
			iterator_type m_Current;
			iterator_type m_Previous;
		};
	}

	template<typename StringLike>
	auto tokenize(const StringLike& text, typename StringLike::value_type value)
	{
		using char_type = typename StringLike::value_type;
		using view_type = std::basic_string_view<char_type>;

		return detail::tokenizer_view<char_type>(view_type(text), value);
	}

}
