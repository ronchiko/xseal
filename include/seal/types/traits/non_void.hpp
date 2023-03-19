#pragma once

#include <type_traits>

namespace seal {

#pragma pack(push)
#pragma pack(1)
	template<typename T>
	struct _unusable
	{
		_unusable() = default;

		_unusable(T value)
			: value(std::move(value))
		{}

		T value;
	};
#pragma pack(pop)

	/**
	 * Ensures a type is not void.
	 */
	template<typename T, typename Alt = char>
	using _non_void = std::conditional_t<std::is_void_v<T>, _unusable<Alt>, T>;

	template<typename T>
	static _non_void<T> _void_dummy_v;
}
