#include "seal/opt/align_alloc.hpp"

#include <cstdlib>

#include "seal/opt/math.hpp"

namespace seal {
#ifdef _MSC_VER
	void* aligned_alloc(size_t alignment, size_t size) {
		return _aligned_malloc(size, std::bit_ceil(alignment));
	}

	void aligned_free(void* address) {
		_aligned_free(address);
	}

#else
	void *aligned_alloc(size_t alignment, size_t size)
	{
		return std::aligned_alloc(std::bit_ceil(alignment), seal::align(size, alignment));
	}

	void aligned_free(void *address)
	{
		std::free(address);
	}

#endif
}
