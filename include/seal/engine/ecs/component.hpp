#pragma once

#include "seal/types.hpp"
#include "seal/types/id_provider.hpp"

namespace seal::ecs {
	template<typename T>
	struct component_traits
	{
		constexpr static size_t SIZE = sizeof(T);

		/**
		   Gets the id of the component.
		 */
		static seal::id id() noexcept;
	};

	template<>
	struct component_traits<void>
	{
		/**
		   Generates a unique id.
		 */
		static seal::id id() noexcept {
			static id_provider s_Provider;

			return s_Provider.make_id();
		}
	};
}

template<typename T>
seal::id seal::ecs::component_traits<T>::id() noexcept
{
	static seal::id s_Id = seal::INVALID_ID;

	if (seal::INVALID_ID == s_Id) {
		s_Id = ecs::component_traits<void>::id();
	}

	return s_Id;
}
