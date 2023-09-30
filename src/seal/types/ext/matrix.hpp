#pragma once

#include <type_traits>

#include <glm/glm.hpp>

namespace seal {
	namespace detail {
		template<typename TypeT>
		struct is_matrix : std::false_type
		{};

		template<size_t Width, size_t Height, typename StorageT, glm::qualifier Qualifier>
		struct is_matrix<glm::mat<Width, Height, StorageT, Qualifier>> : std::true_type
		{};

		template<typename TypeT>
		struct matrix_traits;

		template<size_t Width, size_t Height, typename StorageT, glm::qualifier Qualifier>
		struct matrix_traits<glm::mat<Width, Height, StorageT, Qualifier>>
		{
			static constexpr auto width = Width;
			static constexpr auto height = Height;

			static constexpr auto qualifier = Qualifier;

			using element_type = StorageT;
		};
	}

	template<typename TypeT>
	concept matrix = detail::is_matrix<TypeT>::value;

	template<typename TypeT>
	constexpr bool is_matrix_v = matrix<TypeT>;

	template<typename TypeT>
	using matrix_traits = detail::matrix_traits<std::remove_cvref_t<TypeT>>;
}
