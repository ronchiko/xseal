#pragma once

#include "seal/defenitions.h"

namespace seal {
	class render_context
	{
	public:
		explicit render_context();

		~render_context();
	private:
		seal_no_copy(render_context);
		seal_no_move(render_context);
	};
}
