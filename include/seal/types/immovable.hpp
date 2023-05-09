#pragma once

#include "seal/defenitions.h"

namespace seal {
	template<typename T>
	struct immovable : T
	{
	public:
		

	private:
		seal_no_move(immovable);
	};
}
