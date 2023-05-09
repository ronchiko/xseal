#pragma once

#if defined(_DEBUG) && !defined(SEAL_DEBUG)
#define SEAL_DEBUG
#endif

#ifdef SEAL_DEBUG
/**
 * Executes an expression only in debug.
 */
#define seal_debug_only(expr) expr

#ifdef SEAL_WINDOWS
#define SEAL_DEBUGBREAK() __debugbreak()
#elif defined(__clang__) or defined(SEAL_WEBGL)
#define SEAL_DEBUGBREAK() __builtin_debugtrap()
#endif
#else
#define seal_debug_only(expr) expr
#endif

