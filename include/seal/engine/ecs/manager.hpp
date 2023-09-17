#pragma once

#include <functional>

#include "seal/types.hpp"

#include "seal/engine/ecs/system/system_interface.hpp"

namespace seal::ecs::manager {
	using system_function = std::function<void(void *)>;

	class system
	{
	public:
		/**
			Adds a new system with initialization arguments

			\tparam SystemType: The type of the argument.
			\param args...: The initialization arguments to create the system with.
		*/
		template<typename SystemType, typename... Args>
		static system add(Args&&...args)
		{
			auto new_system = detail::system_interface::create<SystemType>(args...);
			g_Systems.push_back(std::move(new_system));

			return system{ (g_Systems.end() - 1)->instance() };
		}
		/**
		   Adds an update listener for this system.

		   \param listener: The listener to add.
		   \param priority: The priority of the listeners dispatch.
		 */
		void update_listener(const system_function& listener, u32 priority) const;

		void *instance() const;

	private:
		friend class engine;

		static inline std::vector<detail::system_interface> g_Systems = {};

		explicit system(void* instance)
			: m_System(instance)
		{}

		void *m_System;
	};

	/*
		Locks the manager, preventing modification to its state.
	 */
	void lock();

	/**
	   Trigger a tick (Invokes all the udpdate methods).

	   \param delta_time: The time that passed between the last tick and this tick.
	 */
	void tick(float delta_time);
}
