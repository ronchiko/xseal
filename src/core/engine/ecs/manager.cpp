#include "seal/engine/ecs/manager.hpp"

#include <memory>

#include "seal/engine/ecs/system/system_interface.hpp"

namespace seal::ecs::manager {

	struct listener
	{
		void *system;
		manager::system_function function;
	};

	std::vector<listener> s_UpdateListeners;
	std::vector<listener> s_UpdateOnceListeners;

	void system::update_listener(manager::system_function func)
	{
		s_UpdateListeners.push_back(listener{ m_System, func });
	}

	void system::update_once_listener(manager::system_function func)
	{
		s_UpdateOnceListeners.push_back(listener{ m_System, func });
	}

	void *system::instance()
	{
		return m_System;
	}

	void tick(float deltaTime)
	{
		for(const auto& listener : s_UpdateOnceListeners) {
			listener.function(listener.system);
		}

		for(const auto& listener : s_UpdateListeners) {
			listener.function(listener.system);
		}
	}
}
