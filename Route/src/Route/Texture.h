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

	struct TextureInfo
	{
		TextureType type;
		TextureFormat format;
		Vec3u extent;
		uint32_t mip_levels;
	};

	class Texture : public GraphicsResource
	{
	public:
		inline Texture(GraphicsFactory &factory) : GraphicsResource(factory) {
		}
		Texture( const Texture & );

		inline TextureID get_id() const {
			return m_id;
		}

	private:
		TextureID m_id;
		TextureInfo m_info;
	};
}
