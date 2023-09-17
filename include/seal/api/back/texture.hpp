#pragma once

#include "seal/api/buffer.hpp"
#include "seal/api/tagged_object.hpp"


namespace seal::api {
	enum class texture_type
	{
		Texture2d,
	};

	enum class texture_wrap_mode
	{
		Repeat,
		Clamp,
	};

	enum class texture_scale_mode
	{
		Linear,
		Nearest
	};

	struct texture_load_info
	{
		texture_type type;
		seal::api::buffer pixels;
		u32 width;
		u32 height;
		u32 channels;
		texture_wrap_mode wrapping = texture_wrap_mode::Repeat;
		texture_scale_mode scaling = texture_scale_mode::Linear;
		bool generate_mip_maps = true;
	};

	inline constexpr u32 FAILED_TO_BIND_TEXTURE = static_cast<u32>(-1);
	inline constexpr u32 ANY_TEXTURE_INDEX = static_cast<u32>(-1);

	/**
	   Loads a texture into memory.

	   @param load_info - Information about the texture to load.
	 */
	api::abstract_t load_texture(const texture_load_info& load_info);

	/**
	   Unloads a texture from memory.
	 */
	void unload_texture(api::abstract_t texture);

	/**
	   Binds a texture as an active texture, and returns its index.

	   \param texture: The texture to load.
	   \param request_index: The index of the texture to bind, or ANY_TEXTURE_INDEX if we dont need a specific index.

	   \note: If an index is specified, the backend should override the loaded texture at that index.
			  If not, the the backend should return an error if all possible indexes are taken.

	   \returns: The index of the loaded texture or FAILED_TO_BIND_TEXTURE on failure.
	*/
	u32 bind_texture(api::abstract_t texture, u32 request_index = ANY_TEXTURE_INDEX);

	/**
	   Clears all the bound textures.
	 */
	void clear_bound_textures();
}
