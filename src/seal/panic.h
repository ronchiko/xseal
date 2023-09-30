#pragma once

#include <stdexcept>

#include "seal/debug.h"
#include "seal/log/log.hpp"

namespace seal {
	class panic_error : public std::runtime_error
	{
	public:
		explicit panic_error(const char *message)
			: std::runtime_error(message)
		{}
	};
}

/**
 * Prints a message and throws an exception
 */
#define seal_panic(message)                                                                        \
	SEAL_DEBUGBREAK();                                                                             \
	throw ::seal::panic_error((message))


#ifdef SEAL_DEBUG

#define seal_assert(expr, message)                                                                 \
	if(!(expr)) {                                                                                  \
		seal_panic(message);                                                                       \
	}

#define seal_wassert(expr, message)                                                                \
	if(!(expr)) {                                                                                  \
		seal::log::warning(message);                                                               \
	}

#else

#define seal_wassert(expr, message)

#define seal_assert(...)

#endif // SEAL_DEBUG
