#pragma once


namespace route
{
	enum class TextureType
	{
		Texture1D,
		Texture2D,
		Texture3D,
		CubeMap,
	};

	typedef unsigned TextureID;

	class Texture
	{
	public:
		Texture( const Texture & );

	private:
		TextureID m_id;
		TextureType m_type;
	};
}
