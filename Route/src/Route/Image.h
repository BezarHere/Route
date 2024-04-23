#pragma once
#include "Vector.h"
#include "Color.h"

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
		using pixel_pos = Vec2us;

		Image( pixel_pos size );
		Image( const Image &copy );
		~Image();

		pixel_pos size() const;
		uint32_t width() const;
		uint32_t height() const;

		uint32_t *at( pixel_pos position );
		const uint32_t *at( pixel_pos position ) const;

		void put_pixel( pixel_pos position, const Clr &color );
		const Clr &get_pixel( pixel_pos position ) const;

		void put_pixel8( pixel_pos position, Clr8 color );
		Clr8 get_pixel8( pixel_pos position ) const;

		void put_pixel16( pixel_pos position, const Clr16 &color );
		const Clr16 &get_pixel16( pixel_pos position ) const;

	private:
		struct State;
		void *m_surface;
	};
}
