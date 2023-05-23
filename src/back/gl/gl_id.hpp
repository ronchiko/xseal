#pragma once

#include <memory>

#include "gl.hpp"

#include "seal/defenitions.h"
#include "seal/types.hpp"

namespace seal::gl {

	struct gl_id
	{
	public:
#ifdef SEAL_WEBGL
		using deletor = void GLAD_API_PTR (GLuint id);
#else
		using deletor = void (GLAPIENTRY)(GLuint id);
#endif
		constexpr gl_id() noexcept
			: gl_id(INVALID_ID)
		{}

		constexpr gl_id(GLuint id, deletor *del = nullptr) noexcept
			: m_Id(id)
			, m_Deletor(del)
		{}

		seal_no_copy(gl_id);

		constexpr gl_id(gl_id&& other) noexcept
			: m_Id(other.m_Id)
			, m_Deletor(other.m_Deletor)
		{
			other.m_Id = INVALID_ID;
		}

		gl_id& operator=(gl_id&& other) noexcept
		{
			seal_mute_exceptions({
				other.m_Id = std::exchange(m_Id, other.m_Id);
				other.m_Deletor = std::exchange(m_Deletor, other.m_Deletor);
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
				if(m_Deletor && INVALID_ID != m_Id) {
					m_Deletor(m_Id);
				}
				m_Id = INVALID_ID;
			});
		}

		deletor *m_Deletor;
		GLuint m_Id;
	};
}
