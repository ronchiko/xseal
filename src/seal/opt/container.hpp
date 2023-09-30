#pragma once

#include "seal/types/traits/container.hpp"

namespace seal {
	/**
	   Gets the data inside the pointer as a diffrent object.
	  
	   \param container: The
	 * container to get the data from.
	   \tparam T: The type to view the data as
	 */
	template<typename T>
	constexpr T *view_as(seal::container auto& container)
	{
		return reinterpret_cast<T *>(container.data());
	}

	/**
	   Gets the size of a container in bytes.

	   \param container: The container to get the size of.
	 */
	constexpr size_t size_bytes(seal::container auto& container)
	{
		using element_t = typename seal::container_traits<std::remove_cvref_t<decltype(container)>>::element_t;

		return container.size() * sizeof(element_t);
	}
}
