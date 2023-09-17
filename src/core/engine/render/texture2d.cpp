#include "seal/engine/render/texture2d.hpp"

#include <stb_image.h>

#include "seal/defenitions.h"
#include "seal/api/back/texture.hpp"

namespace seal {

	texture2d::~texture2d()
	{
		seal_mute_exceptions({ release(); });
	}

	void texture2d::release()
	{
		if(seal_is_error(m_Texture)) {
			return;
		}

		api::unload_texture(m_Texture);
		m_Texture = error_value_v<api::abstract_t>;
	}

	texture2d texture2d::create(const v2<u32> dimensions, void *pixels, const u32 channels)
	{
		seal::api::texture_load_info load_info{};
		load_info.pixels = api::buffer::bind(pixels, static_cast<size_t>(dimensions.x) * dimensions.y * channels);
		load_info.width = dimensions.x;
		load_info.height = dimensions.y;
		load_info.channels = channels;

		return texture2d{ dimensions, channels, api::load_texture(load_info) };
	}

	u32 texture2d::bind(const u32 desired_index) const
	{
		return api::bind_texture(m_Texture, desired_index);
	}

	std::shared_ptr<texture2d> resource_loader<texture2d>::load(const resource& resource)
	{
		stbi_set_flip_vertically_on_load(true);

		const auto resource_data = resource->load_all();

		int width = 0, height = 0, channels = 0;
		const auto buffer = stbi_load_from_memory(resource_data.data(),
												  static_cast<int>(resource_data.size_bytes()),
												  &width,
												  &height,
												  &channels,
												  STBI_rgb_alpha);
		if(nullptr == buffer) {
			throw seal::failure("Failed to load image, STBI Error: {}", stbi_failure_reason());
		}

		std::unique_ptr<void, decltype(&stbi_image_free)> guard(buffer, &stbi_image_free);

		return std::make_shared<texture2d>(texture2d::create(v2<u32>{ width, height },
															 buffer,
															 channels));
	}
}
