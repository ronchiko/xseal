#pragma once

#include "seal/api/back/texture.hpp"

#include "gl_id.hpp"
#include "tags.h"

namespace seal::gl {

	class texture
	{
	public:
		/**
		   Creates a new texture from a texture loading info.

		   \param info: The info about the texture.
		 */
		static texture create(const api::texture_load_info& info);

		/**
		   Binds the texture to be the active one.
		 */
		void bind() const;

		/**
		   The id of the texture.
		 */
		constexpr u32 id() const;

	private:
		explicit texture(u32 width, u32 height, gl_id id);

		u32 m_Width;
		u32 m_Height;
		gl_id m_Id;
	};
}

constexpr seal::u32 seal::gl::texture::id() const {
	return static_cast<u32>(m_Id);
}

SEAL_TAG_TYPE(seal::gl::texture, seal::gl::tags::Texture);