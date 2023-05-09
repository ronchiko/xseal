#pragma once

#include <span>

#include "seal/opt/math.hpp"

#include "seal/types/traits/container.hpp"

namespace seal::api {
	struct buffer
	{
		size_t size;
		void *data;

		/**
			Views the buffer as a buffer of type.

			\note: If the size of the buffer doesn't align with sizeof(T), then align it downwards.
		 */
		template<typename T>
		constexpr std::span<T> view_as();

		/**
		   Binds a container to a buffer.

		   \param container: The container to bind.
		 */
		constexpr static buffer bind(container auto& container);
	};
}

template<typename T>
constexpr std::span<T> seal::api::buffer::view_as()
{
	return std::span<T>{
		reinterpret_cast<T *>(data),
		size / sizeof(T),
	};
}

constexpr seal::api::buffer seal::api::buffer::bind(seal::container auto& container)
{
	using container_t = std::remove_cvref_t<decltype(container)>;
	constexpr auto element_size = sizeof(typename container_traits<container_t>::element_t);

	return buffer{
		element_size * container.size(),
		container.data(),
	};
}
