#include "seal/engine/systems/all.hpp"

#include "seal/types.hpp"
#include "seal/engine/ecs/system.hpp"
#include "seal/engine/systems/sprite_renderer.hpp"
#include "seal/engine/render/pipeline/pipeline_data_provider.hpp"

struct camera
{
	seal::m4<seal::f32> project;
	seal::m4<seal::f32> view;
};

void seal::detail::register_all_builtin() {
	pipeline_data_provider<camera>::create("seal-global-matrices").update(camera{});

	log::info("Initialized global matrices");

	seal::ecs::register_system<system::sprite_renderer>();
}
