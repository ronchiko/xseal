#pragma once

#include <memory>

#include "gl.hpp"

#include "seal/defenitions.h"
#include "seal/types.hpp"

namespace seal::gl {

	class gl_id
	{
	public:
		using deleter_t = void GLAPIENTRY (GLuint id);

		constexpr gl_id() noexcept
			: gl_id(INVALID_ID)
		{}

		constexpr gl_id(const GLuint id, deleter_t *del = nullptr) noexcept
			: m_Id(id)
			, m_Deleter(del)
		{}

		gl_id(const gl_id&) = delete;
		gl_id& operator=(const gl_id&) = delete;

		constexpr gl_id(gl_id&& other) noexcept
			: m_Id(other.m_Id)
			, m_Deleter(other.m_Deleter)
		{
			other.m_Id = INVALID_ID;
		}

		gl_id& operator=(gl_id&& other) noexcept
		{
			seal_mute_exceptions({
				other.m_Id = std::exchange(m_Id, other.m_Id);
				other.m_Deleter = std::exchange(m_Deleter, other.m_Deleter);
			});

			return *this;
		}

		~gl_id() noexcept
		{
			seal_mute_exceptions({ release(); });
			m_Id = INVALID_ID;
		}

		constexpr operator GLuint() const
		{
			return m_Id;
		}

	private:
		void release() noexcept
		{
			seal_mute_exceptions({
				if(m_Deleter && INVALID_ID != m_Id) {
					m_Deleter(m_Id);
				}
				m_Id = INVALID_ID;
			});
		}

		GLuint m_Id;
		deleter_t *m_Deleter;
	};
}
