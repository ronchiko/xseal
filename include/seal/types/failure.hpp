#pragma once

#include <ostream>

#include <fmt/format.h>

namespace seal {
	template<typename T, typename E>
	class result;

	/**
	 * Represents a failure.
	 */
	class failure
	{
	public:
		constexpr explicit failure(std::string message);

		template<typename... Fmt>
		explicit failure(std::string message, Fmt...);


		/// Implemented in result.hpp
		template<typename T>
		constexpr operator seal::result<T, seal::failure>();

		/**
		 * The message of the failure.
		 */
		constexpr const char *what() const;

	private:
		std::string m_message;
	};
}

constexpr seal::failure::failure(std::string message)
	: m_message(std::move(message))
{}

template<typename... Fmt>
inline seal::failure::failure(std::string message, Fmt... rest)
	: m_message(fmt::format(message, rest...))
{}

constexpr const char *seal::failure::what() const
{
	return m_message.c_str();
}

inline std::ostream& operator<<(std::ostream& stream, const seal::failure& failure)
{
	return stream << failure.what();
}
