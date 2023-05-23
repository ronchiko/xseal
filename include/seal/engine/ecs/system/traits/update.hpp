#pragma once

#include <tuple>
#include <type_traits>

#include "seal/types/ext/result.hpp"

#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/ecs/system/traits/method.hpp"

namespace seal::ecs {
	namespace detail {
		// clang-format off
		template<typename T>
		concept has_update_method = requires {
			{ &T::update };
		};
		// clang-format oon

		template<typename Tuple>
		using first_t = std::tuple_element_t<0, Tuple>;

		template<typename Tuple>
		struct update_argument_pack_ok
			: std::integral_constant<bool, (std::tuple_size_v<Tuple> > 0) && 
			(std::is_same_v<first_t<Tuple>, const entity&> || std::is_same_v<first_t<Tuple>, entity>)>
		{};

		template<typename FunctionT>
		struct update_method_traits {
		private:
			using traits_t = method_traits<FunctionT>;
		public:
			using tuple_t = traits_t::tuple_t;

		private:
			static constexpr bool return_ok_v = is_void_result_v<typename traits_t::return_t>;
			static constexpr bool first_argument_ok_v = update_argument_pack_ok<tuple_t>::value;

		public:
			static constexpr bool is_ok_v = return_ok_v && first_argument_ok_v;
		};

		template<typename SystemT>
		struct safe_update_method_traits
		{
		private:
			static_assert(has_update_method<SystemT>, "System doesn't have an update method");

			using traits_t = update_method_traits<decltype(&SystemT::update)>;
		public:
			static constexpr auto ok_v = traits_t::is_ok_v;

			using tuple_t = typename traits_t::tuple_t;
		};
	}

	template<typename SystemT>
	static constexpr bool has_update_method_v = detail::has_update_method<SystemT>;

	template<typename SystemT>
	static constexpr bool update_ok_v = detail::safe_update_method_traits<SystemT>::ok_v;

	template<typename SystemT>
	using update_tuple_t = detail::safe_update_method_traits<SystemT>::tuple_t;
}
