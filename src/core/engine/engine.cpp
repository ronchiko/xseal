#include "seal/engine/engine.hpp"

#include "seal/defenitions.h"
#include "seal/api/back/backend.hpp"
#include "seal/engine/ecs/manager.hpp"
#include "seal/engine/ecs/entity.hpp"
#include "seal/engine/systems/all.hpp"
#include "seal/types/early_destroy.hpp"

extern void register_all_embedded_resources();
extern void register_global_resources();

namespace seal {

	void seal::engine::create()
	{
		// First initialize the logger.
		seal::log::initialize();
		
		// Then register all the embedded resources because they might be used in the later steps.
		register_global_resources();
		register_all_embedded_resources();

		// Initialize the backend engine.
		api::initialize_backend();

		// Clean en entity registry.
		seal::entity::g_Registry.clear();
		seal::early_destroy::queue(seal::ecs::manager::system::g_Systems);

		// Register all builtin systems last, so they could use the backend/resource manager to perform logic.
		detail::register_all_builtin();

		// Lock the manager since we created our engine.
		seal::ecs::manager::lock();
	}

	void seal::engine::tick(const float delta_time)
	{
		api::start_context(seal::BLACK);

		seal_mute_exceptions({ seal::ecs::manager::tick(delta_time); });
		api::end_context();
	}

	void seal::engine::stop() {
		seal::entity::g_Registry.clear();
		seal::early_destroy::destroy_all();
	}
}
