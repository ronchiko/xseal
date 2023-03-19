#pragma once

#if defined(_DEBUG) && !defined(SEAL_DEBUG)
#define SEAL_DEBUG
#endif

#ifdef SEAL_DEBUG
/**
 * Executes an expression only in debug.
 */
#define seal_debug_only(expr) expr
#else
#define seal_debug_only(expr) expr
#endif

