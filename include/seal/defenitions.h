#pragma once

/*
 ADDITIONAL DEFENITIONS
  - SEAL_WEBGL - When building for WebGL
  - SEAL_WIN32 - When building for Windows x86
  - SEAL_X64 - When building for Windows x64

  - SEAL_DEBUG - When building in debug
 */

#if defined(_MSC_VER)
#define seal_force_inline __forceinline
#elif defined(GCC)
#define seal_force_inline __attribute__((always_inline))
#else
#define seal_force_inline
#endif


#define seal_mute_exceptions(...)                                                                  \
	try {                                                                                          \
		__VA_ARGS__;                                                                               \
	} catch(const std::exception& err) {                                                           \
		seal::log::error("Exception: {}", err.what());                                             \
	} catch(...) {}


#define seal_no_move(cls)                                                                          \
	cls(cls&&) noexcept = delete;                                                                  \
	cls& operator=(cls&&) noexcept = delete;

#define seal_no_copy(cls)                                                                          \
	cls(const cls&) noexcept = delete;                                                             \
	cls& operator=(const cls&) noexcept = delete;
