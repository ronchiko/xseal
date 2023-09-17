#pragma once

#include "seal/debug.h"
#include "seal/panic.h"

#include "seal/types.hpp"
#include "seal/types/error_value.hpp"

namespace seal::api {
	/**
	   The integral type repsenting a tag.

	   The tag ranges are:
	   0 - 1000		: reserved
	   1000 - 2000	: OpenGL backend
	 */
	using tag_t = u16;

	template<typename T>
	struct tagged_type;

	enum Tags : tag_t
	{
		TAG_NONE = 0
	};

	/**
	   An object to help check the types of data passed using the API.

	   \note: This object does not own the inner resource. Make sure you free them.
	 */
	class tagged_object final
	{
	public:
		constexpr tagged_object(tag_t tag, void *data)
			: m_Tag(tag)
			, m_Data(data)
		{}

		constexpr tagged_object(const tagged_object&) = default;
		constexpr tagged_object(tagged_object&& other) noexcept
			: m_Tag(other.m_Tag)
			, m_Data(other.m_Data)
		{
			other.m_Data = nullptr;
			other.m_Tag = TAG_NONE;
		}

		constexpr tagged_object& operator=(const tagged_object&) = default;

		constexpr tagged_object& operator=(tagged_object&& other) noexcept {
			std::swap(m_Tag, other.m_Tag);
			std::swap(m_Data, other.m_Data);

			return *this;
		}

		constexpr ~tagged_object() = default;

		template<typename T>
		constexpr static tagged_object bind(T* object)
		{
			return tagged_object{ tagged_type<T>::tag_v, object };
		}

		/**
		   Asserts the tag of the object is a good tag.
		 */
		template<typename T>
		constexpr const T *acquire() const
		{
			assert_tag<T>();
			return static_cast<const T *>(m_Data);
		}

		template<typename T>
		constexpr T *acquire()
		{
			assert_tag<T>();
			return static_cast<T *>(m_Data);
		}

		constexpr bool operator==(const tagged_object& other) const {
			return m_Tag == other.m_Tag && m_Data == other.m_Data;
		}

		constexpr bool operator!=(const tagged_object& other) const {
			return m_Tag != other.m_Tag && m_Data != other.m_Data;
		}

	private:
		template<typename T>
		constexpr void assert_tag() const
		{
			seal_assert(tagged_type<T>::tag_v == m_Tag, "Got object with invalid tag!");
		}

		tag_t m_Tag;
		void *m_Data;
	};

	using abstract_t = tagged_object;

	inline constexpr tagged_object UNTAGGED_OBJECT{ static_cast<tag_t>(TAG_NONE), nullptr };
}

// Implement error_value
template<>
struct seal::error_value<seal::api::tagged_object>
{
	static constexpr seal::api::tagged_object VALUE = seal::api::UNTAGGED_OBJECT;
};

#define SEAL_TAG_TYPE(type, tag)                                                                   \
	template<>                                                                                     \
	struct ::seal::api::tagged_type<type>                                                          \
	{                                                                                              \
		static constexpr tag_t tag_v = static_cast<seal::api::tag_t>(tag);                         \
	}
