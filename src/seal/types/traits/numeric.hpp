#pragma once

#include <concepts>

namespace seal {
	/**
	   A numeric value (floating point or integral).
	 */
	template<typename T>
	concept numeric = std::integral<T> || std::floating_point<T>;
}
