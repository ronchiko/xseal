#pragma once

#include "seal/engine/ecs/system/traits/method.hpp"

namespace seal::ecs {
	namespace detail {
		// clang-format off
		template<typename SystemT>
		concept has_initialize_method = requires {
			{ &SystemT::initialize };
		};

		// clang-format on

		template<typename FunctionT>
		struct initialize_method_traits
		{
		private:
			using traits_t = method_traits<FunctionT>;
			using tuple_t = traits_t::tuple_t;

			static constexpr bool return_ok_v = std::is_void_v<typename traits_t::return_t>;
			static constexpr bool arguments_ok_v = (std::tuple_size_v<tuple_t>) <= 0;

		public:
			static constexpr auto is_ok_v = return_ok_v && arguments_ok_v;
		};

		template<typename SystemT>
		struct safe_initialize_method_traits
		{
		private:
			static_assert(has_initialize_method<SystemT>,
						  "System doesn't have an initialize method.");

			using traits_t = initialize_method_traits<decltype(&SystemT::initialize)>;

		public:
			static constexpr auto ok_v = traits_t::is_ok_v;
		};
	}

	template<typename SystemT>
	constexpr auto has_initialize_method_v = detail::has_initialize_method<SystemT>;

	template<typename SystemT>
	constexpr auto initialize_method_ok_v = detail::safe_initialize_method_traits<SystemT>::ok_v;
}
