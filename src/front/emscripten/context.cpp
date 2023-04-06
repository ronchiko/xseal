#include "context.h"

namespace seal::ems {
	context& context::get() {
		if (nullptr == g_Context) {
			g_Context = std::make_unique<context>();
		}

		return *g_Context;
	}
}
