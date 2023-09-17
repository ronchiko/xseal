#include "context.h"

namespace seal::ems {
	context& context::get() {
		if (nullptr == s_Context) {
			s_Context = std::make_unique<context>();
		}

		return *s_Context;
	}
}
