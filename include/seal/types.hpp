#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace seal {
	using id = std::int32_t;

	using i32 = std::int32_t;
	using u32 = std::uint32_t;

	using f32 = float;

	using vec2 = glm::vec<2, f32>;
	using vec3 = glm::vec<3, f32>;
	using vec4 = glm::vec<4, f32>;

	using int2 = glm::vec<2, i32>;
	using int3 = glm::vec<3, i32>;
	using int4 = glm::vec<4, i32>;

	struct ansi_string
	{
		constexpr ansi_string(nullptr_t)
			: buffer(nullptr)
			, size(0)
		{}

		constexpr ansi_string(char *buffer)
			: ansi_string(buffer, _string_length(buffer))
		{}

		constexpr ansi_string(char *buffer, size_t size)
			: buffer(buffer)
			, size(size)
		{}

		char *buffer;
		size_t size;

	private:
		constexpr static size_t _string_length(char *buffer)
		{
			size_t length = 0;
			// Until a null-terminator is found
			for(; *buffer; ++buffer, ++length) {}

			return length;
		}
	};
}
