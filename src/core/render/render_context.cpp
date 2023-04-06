#include "seal/render/context.hpp"

#include "seal/api/back/backend.hpp"

namespace seal {
	render_context::render_context() {
		api::start_context(color::rgb(1, 0, 0));
	}

	render_context::~render_context() {
		api::end_context();
	}
}
