#pragma once

#include <array>
#include <concepts>
#include <set>
#include <span>
#include <unordered_set>
#include <vector>

#include "seal/types/buffer.hpp"

namespace seal {

	template<typename T>
	struct container_traits;

	template<typename T>
	struct container_traits<std::vector<T>>
	{
		using element_t = T;
	};

	template<typename T, size_t S>
	struct container_traits<std::span<T, S>>
	{
		using element_t = T;
	};

	template<typename T>
	struct container_traits<std::set<T>>
	{
		using element_t = T;
	};

	template<typename T>
	struct container_traits<std::unordered_set<T>>
	{
		using element_t = T;
	};

	template<typename T, size_t S>
	struct container_traits<std::array<T, S>>
	{
		using element_t = T;
	};

	template<typename T>
	struct container_traits<seal::buffer<T>>
	{
		using element_t = T;
	};

	// clang-format off
	template<typename T>
	concept container = requires(T value) {
							{ value.size() } -> std::same_as<size_t>;
							{ value.data() } -> std::same_as<typename container_traits<T>::element_t*>;
							{ value.begin() };
							{ value.end() };
						};
	// clang-format on
}
