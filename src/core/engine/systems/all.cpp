#include "seal/engine/systems/all.hpp"

#include "seal/engine/ecs/system.hpp"
#include "seal/engine/systems/sprite_renderer.hpp"
#include "seal/engine/systems/camera/camera_manager.hpp"

namespace 
{
	void create_main_camera()
	{
		auto entity = seal::entity::create();

		auto& transform = entity.add<seal::transform>();
		auto& camera = entity.add<seal::camera>();
		camera = seal::camera::create(seal::camera::orthographic{
			10
		});

		transform.position.z = 0.5f;
		// transform.rotation = seal::quat(seal::v3<seal::f32>{ 0, glm::radians(30.f), 0 });

		camera.set_state(seal::camera_state::Active);
	}
}

void seal::detail::register_all_builtin() {
	log::info("Initialized global matrices");

	create_main_camera();

	seal::ecs::register_system<system::camera_manager>();
	seal::ecs::register_system<system::sprite_renderer>();
}
