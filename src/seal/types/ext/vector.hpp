#pragma once

#include <type_traits>

#include <glm/glm.hpp>

namespace seal {
	namespace detail {
		template<typename TypeT>
		struct is_vector : std::false_type
		{};

		template<size_t Size, typename StorageT, glm::qualifier Qualifier>
		struct is_vector<glm::vec<Size, StorageT, Qualifier>> : std::true_type
		{};

		template<typename TypeT>
		struct vector_traits;

		template<size_t Size, typename StorageT, glm::qualifier Qualifier>
		struct vector_traits<glm::vec<Size, StorageT, Qualifier>>
		{
			static constexpr auto size = Size;

			using element_type = StorageT;
		};
	}

	template<typename TypeT>
	concept vector = detail::is_vector<TypeT>::value;

	template<typename TypeT>
	constexpr bool is_vector_v = vector<TypeT>;

	template<typename TypeT>
	using vector_traits = detail::vector_traits<std::remove_cvref_t<TypeT>>;
}
