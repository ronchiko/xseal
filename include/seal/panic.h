#pragma once

#include "seal/log/log.hpp"

#if SEAL_ENABLE_EXCEPTIONS

#include <stdexcept>

namespace seal {
	class panic_error : public std::exception
	{
	public:
		explicit panic_error(const char *message)
			: std::exception(message)
		{}
	};
}

/**
 * Prints a message and throws an exception
 */
#define seal_panic(message) throw ::seal::panic_error((message))

#else

#include <cstdio>
#include <cstdlib>
#include <string>

#define SEAL_PANICED 100

namespace seal {
	template<typename T>
	struct _explicit_string
	{
		explicit _explicit_string(const T& value)
			: value(static_cast<std::string>(value))
		{}

		operator const char *() const
		{
			return value.c_str();
		}

		std::string value;
	};

	template<>
	struct _explicit_string<const char *>
	{
		constexpr explicit _explicit_string(const char *value)
			: value(value)
		{}

		constexpr operator const char *() const
		{
			return value;
		}

		const char *value;
	};
}

/**
 * Prints a message and ends the process.
 */
#define seal_panic(message)                                                                        \
	{                                                                                              \
		::seal::log::error(::seal::_explicit_string(message));                                     \
		::std::exit(SEAL_PANICED);                                                                 \
	}
#endif

#define seal_assert(expr, message)                                                                 \
	if(!(expr)) {                                                                                  \
		seal_panic(message);                                                                       \
	}
