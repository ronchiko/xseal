#pragma once

#include "seal/api/back/texture.hpp"
#include "seal/api/tagged_object.hpp"
#include "seal/engine/resource.hpp"
#include "seal/types.hpp"

namespace seal {

	class texture2d
	{
	public:
		constexpr texture2d(texture2d&& other) noexcept = default;
		texture2d& operator=(texture2d&& other) noexcept = default;

		texture2d(const texture2d&) = delete;
		texture2d& operator=(const texture2d&) = delete;

		~texture2d();

		/**
		   Creates a new texture from a its dimensions and pixel data.
		 */
		static texture2d create(v2<u32> dimensions, void *pixels, u32 channels);

		/**
			Binds the texture to an index. Returns the index of the bound texture.

			\see api::bind_texture
		 */
		[[nodiscard]] u32 bind(u32 desired_index = api::ANY_TEXTURE_INDEX) const;

		/**
		   The width of the texture.
		 */
		[[nodiscard]] constexpr u32 width() const;

		/**
		   The height of the texture.
		 */
		[[nodiscard]] constexpr u32 height() const;

		/**
			The amount of channels in the texture
		 */
		[[nodiscard]] constexpr u32 channels() const;

		/**
			Returns the abstracted type of this texture.
		 */
		inline api::abstract_t abstracted() const;

		constexpr bool operator==(const texture2d& other) const;
		constexpr bool operator!=(const texture2d& other) const;

	private:
		/**
			Frees the texture
		 */
		void release();

		/**
			Creates a new texture2d object from its and a bound texture
		*/
		constexpr texture2d(const v2<u32> dimensions, const u32 channels, api::abstract_t bound_texture)
			: m_Width(dimensions.x)
			, m_Height(dimensions.y)
			, m_Channels(channels)
			, m_Texture(std::move(bound_texture))
		{}

		u32 m_Width;
		u32 m_Height;
		u32 m_Channels;
		api::abstract_t m_Texture;
	};
}

template<>
struct seal::resource_loader<seal::texture2d>
{
public:
	[[nodiscard]] static std::shared_ptr<seal::texture2d> load(const seal::resource& resource);
};

constexpr seal::u32 seal::texture2d::width() const
{
	return m_Width;
}

constexpr seal::u32 seal::texture2d::height() const
{
	return m_Height;
}

constexpr seal::u32 seal::texture2d::channels() const
{
	return m_Channels;
}

inline seal::api::abstract_t seal::texture2d::abstracted() const
{
	return m_Texture;
}

constexpr bool seal::texture2d::operator==(const texture2d& other) const
{
	return m_Texture == other.m_Texture;
}

constexpr bool seal::texture2d::operator!=(const texture2d& other) const
{
	return m_Texture != other.m_Texture;
}

