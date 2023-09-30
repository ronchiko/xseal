#pragma once

#include <cstdint>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

namespace seal {
	using id = std::uint32_t;
	constexpr id INVALID_ID = 0;

	using u8 = std::uint8_t;

	using i16 = std::int16_t;
	using u16 = std::uint16_t;

	using i32 = std::int32_t;
	using u32 = std::uint32_t;

	using i64 = std::int64_t;
	using u64 = std::uint64_t;

	using f32 = float;
	using f64 = double;

	using radians_t = f32; // Species the values should be in radians

	// Vectors

	template<typename TElement>
	using v2 = glm::vec<2, TElement>;

	template<typename TElement>
	using v3 = glm::vec<3, TElement>;

	template<typename TElement>
	using v4 = glm::vec<4, TElement>;

	// Matrices

	template<typename TElement>
	using m3 = glm::mat<3, 3, TElement>;

	template<typename TElement>
	using m4 = glm::mat<4, 4, TElement>;

	/// Quaternion
	using quat = glm::qua<f32>;
	
	inline constexpr v4<f32> UNIT_RECT = { 0, 0, 1, 1 };

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
