#pragma once
#include <string>

#include "api/resource.hpp"

namespace seal {

	template<typename CharType>
	class basic_string_resource
	{
	public:
		using char_type = CharType;
		using string_type = std::basic_string<char_type>;

		using value_type = CharType;
		using view_type = std::basic_string_view<char_type>;
		using owning_type = string_type;

		explicit basic_string_resource(resource resource)
			: m_Resource(std::move(resource))
		{}

		char_type read_char()
		{
			auto c = read(1);
			return c.size() > 0 ? c[0] : '\0';
		}

		view_type read(const u32 bytes)
		{
			auto view = m_Resource->load(bytes);

			return view_type{ reinterpret_cast<const value_type *>(view.data()),
							  view.size_bytes() / sizeof(value_type) };
		}

		string_type read_line()
		{
			std::basic_stringstream<char_type> stream;
			for(char_type c = read_char(); '\n' != c && '\0' != c; c = read_char()) {
				stream << c;
			}

			return stream.str();
		}

		string_type read_token()
		{
			std::basic_stringstream<char_type> stream;
			for(char_type c = read_char(); !is_delimiter(c); c = read_char()) {
				stream << c;
			}

			return stream.str();
		}

	private:
		static constexpr bool is_delimiter(const char_type c)
		{
			return '\n' == c || '\0' == c || ' ' == c || '\t' == c;
		}

		resource m_Resource;
	};

	using string_resource = basic_string_resource<char>;
	using wstring_resource = basic_string_resource<wchar_t>;
}
