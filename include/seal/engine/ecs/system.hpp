#pragma once

#include "seal/types/traits/tuple.hpp"

#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/ecs/manager.hpp"
#include "seal/engine/ecs/system/system_invoker.hpp"

#include "seal/engine/ecs/system/traits/initialize.hpp"
#include "seal/engine/ecs/system/traits/update.hpp"
#include "seal/engine/ecs/system/traits/update_last.hpp"
#include "seal/engine/ecs/system/traits/update_once.hpp"

namespace seal::ecs {

	inline constexpr u32 PRIORITY_LATE = 1000;
	inline constexpr u32 PRIORITY_UPDATE = 500;
	inline constexpr u32 PRIORITY_EARLY = 100;

	namespace detail {
		template<typename Tuple>
		struct do_registry_call;

		/**
		   Does an entt registry call correctly when we have an empty components list.
		 */
		template<typename... Components>
		struct do_registry_call<std::tuple<Components...>>
		{
			entt::basic_registry<seal::entity_id>& registry;

			do_registry_call(entt::basic_registry<seal::entity_id>& registry)
				: registry(registry)
			{}

			template<typename T>
			auto each(T function)
			{
				if constexpr(sizeof...(Components) > 0) {
					return registry.view<std::remove_reference_t<Components>...>().each(function);
				} else {
					// If we have an empty tuple we cant call each from a view and need to use a
					// function.
					return registry.each(function);
				}
			}
		};

		/**
		   A method that invokes the update method of a system.
		 */
		template<typename SystemType, typename Tuple>
		void update_dispatch(void *system)
		{
			auto& instance = *reinterpret_cast<SystemType *>(system);

			do_registry_call<Tuple>{ seal::entity::g_Registry }.each([&instance](entity_id id,
																				 auto&&...rest) {
				instance.update(seal::entity::find(id), rest...);
			});
		}

		template<typename SystemT>
		void update_once_dispatch(void *system)
		{
			reinterpret_cast<SystemT *>(system)->update_once();
		}

		template<typename SystemT>
		void update_last_dispatch(void *system)
		{
			reinterpret_cast<SystemT *>(system)->update_last();
		}
	}

	template<typename T, typename... InitArgs>
	void register_system(InitArgs... args)
	{
		auto system = manager::system::add<T>(args...);

		if constexpr(has_initialize_method_v<T>) {
			static_assert(initialize_method_ok_v<T>,
						  "Got invalid initialize method!"
						  "a system initialize method must return a result<void> and get no arguments.");

			result<void> initialization_status = reinterpret_cast<T *>(system.instance())
													 ->initialize();
			// If initialization has failed, then don't register any method.
			if(initialization_status.is_error()) {
				seal::log::error("Failed to initalize system");
				return;
			}
		}

		if constexpr(has_update_method_v<T>) {
			static_assert(update_ok_v<T>,
						  "Got invalid update method! "
						  "a system update method must return void and get an const entity& or entity as its first argument.");

			using components = seal::tuple_remove_first_element_t<update_tuple_t<T>>;

			system.update_listener(detail::update_dispatch<T, components>, PRIORITY_UPDATE);
		}

		if constexpr(has_update_once_method_v<T>) {
			static_assert(update_once_ok_v<T>,
						  "Got invalid update_once method!"
						  "a system update_once method must return void and get no arguments.");

			system.update_listener(detail::update_once_dispatch<T>, PRIORITY_EARLY);
		}

		if constexpr(has_update_last_method_v<T>) {
			static_assert(update_last_ok_v<T>,
						  "Got invalid update_last method!"
						  "a system update_once method must return void and get no arguments.");

			system.update_listener(detail::update_last_dispatch<T>, PRIORITY_LATE);
		}
	}
}