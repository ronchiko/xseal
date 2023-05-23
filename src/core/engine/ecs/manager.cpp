#include "seal/engine/ecs/manager.hpp"

#include <span>
#include <memory>
#include <algorithm>

#include "seal/log/log.hpp"
#include "seal/engine/ecs/system/system_interface.hpp"

namespace seal::ecs::manager {

	struct callback
	{
		u32 priority;

		void *system;
		manager::system_function function;
	};

	struct callback_manager
	{
	public:
		std::span<callback> view() {
			return std::span{ m_Callbacks };
		}

		void insert(const callback& callback) {
			if(m_IsLocked) {
				seal::log::error("Attemping to insert a callback after locking the manager.");
				return;
			}

			m_Callbacks.push_back(callback);
		}

		void lock() {
			if(m_IsLocked) {
				seal::log::warning("Attempted lock of the manager after it was locked");
				return;
			}
			
			m_IsLocked = true;
			// Sort the callbacks according to their priority
			std::sort(m_Callbacks.begin(),
					  m_Callbacks.end(),
					  [](const callback& first, const callback& second) {
						  return first.priority > second.priority;
					  });
		}

	private:
		bool m_IsLocked;
		std::vector<callback> m_Callbacks;
	};

	static callback_manager s_Callbacks;

	void system::update_listener(manager::system_function func, u32 priority)
	{
		s_Callbacks.insert(callback{ priority, m_System, func });
	}

	void *system::instance()
	{
		return m_System;
	}

	void lock() {
		s_Callbacks.lock();
	}

	void tick(float deltaTime)
	{
		for (const auto& listener : s_Callbacks.view()) {
			listener.function(listener.system);
		}
	}
}
