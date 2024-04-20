#pragma once
#include "Resource.h"


namespace route
{
	enum class TextureType
	{
		Texture1D,
		Texture2D,
		Texture3D,
		CubeMap,
	};

	enum class TextureFormat
	{
		Invalid = -1,

		RGB8,
		RGB16,
		RGBA8,
		RGBA16,
		RGBA32,
		RGBA16F,
		RGBA32F,

		BGR8,
		BGR16,
		BGRA8,
		BGRA16,
		BGRA32,
		BGRA16F,
		BGRA32F,
	};

	typedef vpid_t TextureID;

	class Texture : public GraphicsResource
	{
	public:
		inline Texture() {
		}
		Texture( const Texture & );

	private:
		TextureID m_id;
		TextureType m_type;
		TextureFormat m_format;
		Vec3u m_extent;
		uint32_t m_mip_levels;
	};
}
