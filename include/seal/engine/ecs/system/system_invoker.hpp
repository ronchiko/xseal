#pragma once

#include <tuple>

#include "seal/engine/ecs/entity.hpp"

namespace seal::ecs::detail {

	/**
	   Helper class for invoking system method that require an const entity& for ECS functions.

	   \tparam SystemT: The system type to call.
	   \tparam Tuple: The argument tuple (without the first argument that assumed to be const seal::entity&)
	 */
	template<typename SystemT, typename Tuple>
	class system_invoker;

	template<typename SystemT, typename... Arguments>
	class system_invoker<SystemT, std::tuple<Arguments...>>
	{
	private:
		using tuple_t = std::tuple<entity_id, Arguments...>;
		using function_t = void (SystemT::*)(const entity&, Arguments...);

	public:
		/**
		   Invokes the update method of the system with a tuple of arguments.
		  
		   \param system: The system to invoke the function on.
		   \param args: The arguments to pass to the function.
		 */
		void invoke_update(SystemT& system, const tuple_t& args)
		{
			return invoke<&SystemT::update>(system, args);
		}

	private:
		/**
		   Generic invoke member function of SystemT that return nothing.

		   \tparam Function: The function to call.

		   \param system: The system to call function to.
		   \param args: The arguments to pass to Function
		 */
		template<function_t Function>
		seal_force_inline void invoke(SystemT& system, const tuple_t& args)
		{
			// clang-format off
			std::apply([&system](entity_id id, auto&&...arguments) { 
				(&system->*Function)(entity::find(id), arguments...); 
			}, args);
			// clang-format on
		}
	};
}
