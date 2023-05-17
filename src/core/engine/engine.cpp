#include "seal/engine/engine.hpp"

#include "seal/api/back/backend.hpp"
#include "seal/engine/ecs/manager.hpp"
#include "seal/engine/systems/all.hpp"

extern seal::result<void> register_all_embedded_resources();

namespace seal {

	result<seal::engine> seal::engine::create()
	{
		// First initialize the logger.
		seal_verify_result(seal::log::initialize());
		
		// Then register all the embedded resources because they might be used in the later steps.
		seal_verify_result(register_all_embedded_resources());

		// Initialize the backend engine.
		seal_verify_result(api::initialize_backend());

		// Register all builtin systems last, so they could use the backend/resource manager to perform logic.
		seal_verify_result(detail::register_all_builtin());

		return seal::engine();
	}

	void seal::engine::tick(float deltaTime)
	{
		if(!api::start_context(seal::BLACK)) {
			seal::log::error("Failed to start rendering context");
			return;
		}

		seal_mute_exceptions({ seal::ecs::manager::tick(deltaTime); });
		api::end_context();
	}
}
