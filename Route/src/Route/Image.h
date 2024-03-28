#pragma once
#include "Vector.h"

namespace route
{
	enum class ImageFormat
	{
		BGRA8888,
		BGRA4444,
		BGR888,
		BGR444,

		RGBA8888,
		RGBA4444,
		RGB888,
		RGB444,
	};

	class Image
	{
	public:
		Image( Vec2u size );
		Image( const Image &copy );
		~Image();

		Vec2u size() const;
		uint32_t width() const;
		uint32_t height() const;

		uint32_t *at( const Vec2u &position );
		const uint32_t *at( const Vec2u &position ) const;

	private:
		struct State;
		State *m_state;
	};
}
