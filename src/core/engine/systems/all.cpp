#include "seal/engine/systems/all.hpp"


#include "seal/engine/ecs/system.hpp"
#include "seal/engine/systems/sprite_renderer.hpp"

seal::result<void> seal::detail::register_all_builtin() {
	seal::ecs::register_system<seal::system::sprite_renderer>();

	return {};
}
