#include "seal/engine/ecs/manager.hpp"

#include <span>

#include "seal/log/log.hpp"
#include "seal/types/ranges/bindings.hpp"

namespace seal::ecs::manager {

	struct callback
	{
		u32 priority;

		void *system;
		system_function function;
	};

	struct callback_manager
	{
	public:
		std::span<callback> view()
		{
			return std::span{ m_Callbacks };
		}

		void insert(const callback& callback)
		{
			if(m_IsLocked) {
				log::error("Attempting to insert a callback after locking the manager.");
				return;
			}

			m_Callbacks.push_back(callback);
		}

		void lock()
		{
			if(m_IsLocked) {
				log::warning("Attempted lock of the manager when its already locked");
				return;
			}

			m_IsLocked = true;
			// Sort the callbacks according to their priority
			ranges::sort(m_Callbacks, [](const callback& first, const callback& second) {
				return first.priority > second.priority;
			});
		}

	private:
		// After a we finished initializing the engine the callback manager shouldn't change.
		bool m_IsLocked = false;
		std::vector<callback> m_Callbacks;
	};

	namespace {
		callback_manager g_Callbacks;
	}

	void system::update_listener(const system_function& listener, const u32 priority) const
	{
		g_Callbacks.insert(callback{ priority, m_System, listener });
	}

	void *system::instance() const
	{
		return m_System;
	}

	void lock()
	{
		g_Callbacks.lock();
	}

	void tick(const float delta_time)
	{
		for(const auto& listener : g_Callbacks.view()) {
			listener.function(listener.system);
		}
	}
}
