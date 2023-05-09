#pragma once

#include "seal/types/result.hpp"

namespace seal::detail {
	/**
	   Registers all the builtin systems.
	 */
	result<void> register_all_builtin();
}
