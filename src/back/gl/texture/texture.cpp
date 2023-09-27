#include "texture/texture.hpp"

#include "seal/api/back/texture.hpp"
#include "seal/types/storage.hpp"

namespace seal {
	namespace {
		storage<gl::texture> g_LoadedTextures;
		u32 g_LastBoundTexture = 0;

		constexpr GLint TEXTURE_WARP_OPTIONS[] = {
			/* api::texture_wrap_mode::Repeat = */ GL_REPEAT,
			/* api::texture_wrap_mode::Linear = */ GL_LINEAR,
		};

		constexpr GLint TEXTURE_MAG_FILTERS[] = {
			/* api::texture_scale_mode::Linear = */ GL_LINEAR,
			/* api::texture_scale_mode::Nearest = */ GL_NEAREST,
		};

		constexpr GLint TEXTURE_MIP_MAP_MIN_FILTERS[] = {
			/* api::texture_scale_mode::Linear = */ GL_LINEAR_MIPMAP_LINEAR,
			/* api::texture_scale_mode::Nearest = */ GL_NEAREST_MIPMAP_NEAREST,
		};

		// Runtime checks depend on this condition.
		static_assert(std::size(TEXTURE_MAG_FILTERS) == std::size(TEXTURE_MIP_MAP_MIN_FILTERS));

		GLint select_minimized_filter(const api::texture_scale_mode mode, const bool mip_maps)
		{
			if(!mip_maps) {
				return TEXTURE_MAG_FILTERS[static_cast<u8>(mode)];
			}

			// TODO: Deal with GL_NEAREST_MIPMAP_LINEAR and GL_LINEAR_MIPMAP_NEAREST cases.
			return TEXTURE_MIP_MAP_MIN_FILTERS[static_cast<u8>(mode)];
		}

		void delete_texture(const u32 texture)
		{
			glDeleteTextures(1, &texture);
		}
	}

	gl::texture gl::texture::create(const api::texture_load_info& info)
	{
		u32 id = 0;
		SEAL_GL_VERIFY(glGenTextures(1, &id));

		gl_id id_object(id, &delete_texture);

		SEAL_GL_VERIFY(glBindTexture(GL_TEXTURE_2D, id));

		// Some checks before accessing the arrays.
		const auto warp_mode = static_cast<u8>(info.wrapping);
		const auto scale_mode = static_cast<u8>(info.scaling);
		if(warp_mode >= std::size(TEXTURE_WARP_OPTIONS)) {
			seal::log::error("Invalid texture wrapping option: {}", warp_mode);
			throw seal::failure("Invalid texture wrapping option: {}", warp_mode);
		}

		if(scale_mode >= std::size(TEXTURE_MAG_FILTERS)) {
			seal::log::error("Invalid texture scale mode: {}", scale_mode);
			throw seal::failure("Invalid texture scale mode: {}", scale_mode);
		}

		// Setup texture parameters
		const auto wrapping = TEXTURE_WARP_OPTIONS[static_cast<u8>(info.wrapping)];
		const auto min_filter = select_minimized_filter(info.scaling, info.generate_mip_maps);
		const auto mag_filter = TEXTURE_MAG_FILTERS[static_cast<u8>(info.scaling)];

		SEAL_GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping));
		SEAL_GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping));
		SEAL_GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, min_filter));
		SEAL_GL_VERIFY(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, mag_filter));

		// Set the texture image data
		SEAL_GL_VERIFY(glTexImage2D(GL_TEXTURE_2D,
					 0,
					 GL_RGBA,
					 static_cast<GLsizei>(info.width),
					 static_cast<GLsizei>(info.height),
					 0,
					 GL_RGBA,
					 GL_UNSIGNED_BYTE,
					 info.pixels.data));

		// Generate mip maps
		if(info.generate_mip_maps) {
			SEAL_GL_VERIFY(glGenerateMipmap(GL_TEXTURE_2D));
		}

		return gl::texture(info.width, info.height, std::move(id_object));
	}

	void gl::texture::bind() const
	{
		glBindTexture(GL_TEXTURE_2D, m_Id);
	}

	gl::texture::texture(const u32 width, const u32 height, gl_id id)
		: m_Width(width)
		, m_Height(height)
		, m_Id(std::move(id))
	{}

	api::abstract_t api::load_texture(const texture_load_info& load_info)
	{
		auto& texture = g_LoadedTextures.store(gl::texture::create(load_info));
		return abstract_t::bind(&texture);
	}

	void api::unload_texture(abstract_t texture)
	{
		auto *obj = texture.acquire<gl::texture>();
		g_LoadedTextures.erase(*obj);
	}

	u32 api::bind_texture(api::abstract_t texture, u32 request_index)
	{
		if(ANY_TEXTURE_INDEX == request_index) { // Append to already loaded textures
			// Too many bound textures
			if(g_LastBoundTexture >= static_cast<u32>(gl::specs::max_textures_combined)) {
				seal::log::error("Cant bind new texture because the maximum amount of textures are bound"
								 "({} textures).",
								 gl::specs::max_textures_combined);
				return FAILED_TO_BIND_TEXTURE;
			}

			request_index = g_LastBoundTexture++;
		}

		const auto obj = texture.acquire<gl::texture>();

		// Index too large
		if(request_index >= static_cast<u32>(gl::specs::max_textures_combined)) {
			seal::log::error("Failed to bind texture to index {}, maximum texture count is {}.",
							 request_index,
							 gl::specs::max_textures_combined);
			return FAILED_TO_BIND_TEXTURE;
		}

		glActiveTexture(GL_TEXTURE0 + request_index);
		obj->bind();

		return request_index;
	}

	void api::clear_bound_textures()
	{
		// This actually does nothing, because we don't need to explicitly reset all the bound
		// textures, just reset the texture counter.
		g_LastBoundTexture = 0;
	}
}
