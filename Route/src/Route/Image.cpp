#include "pch.h"
#include "Image.h"
#include <SDL.h>

#define CAST_SF(p) (reinterpret_cast<SDL_Surface *>(p))

static FORCE_INLINE size_t pixel_byte_index( SDL_Surface *surface, uint16_t x, uint16_t y ) {
	return surface->pitch * y + surface->format->BytesPerPixel * x;
}
namespace route
{

	Image::Image( pixel_pos size )
		: m_surface{ SDL_CreateRGBSurfaceWithFormat( 0, size.x, size.y, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 ) } {
		// TODO: error checking
	}

	Image::Image( const Image &copy ) {
		// TODO
	}

	Image::~Image() {
		SDL_FreeSurface( CAST_SF( m_surface ) );
	}

	Image::pixel_pos Image::size() const {
		return pixel_pos( CAST_SF( m_surface )->w, CAST_SF( m_surface )->h );
	}

	uint32_t Image::width() const {
		return CAST_SF( m_surface )->w;
	}

	uint32_t Image::height() const {
		return CAST_SF( m_surface )->h;
	}

	uint32_t *Image::at( const pixel_pos pos ) {
		uint8_t *const pixels = static_cast<uint8_t *>(CAST_SF( m_surface )->pixels);
		return reinterpret_cast<uint32_t *>(&pixels[ pixel_byte_index( CAST_SF( m_surface ), pos.x, pos.y ) ]);
	}

	const uint32_t *Image::at( const pixel_pos pos ) const {
		const uint8_t *const pixels = static_cast<uint8_t *>(CAST_SF( m_surface )->pixels);
		return reinterpret_cast<const uint32_t *>(&pixels[ pixel_byte_index( CAST_SF( m_surface ), pos.x, pos.y ) ]);
	}

	void Image::put_pixel( pixel_pos pos, const Clr &color ) {
		// TODO: code
	}

	const Clr &Image::get_pixel( pixel_pos pos ) const {
		// TODO: code
		return {};
	}

	void Image::put_pixel8( pixel_pos pos, Clr8 color ) {
		// TODO: code
	}

	Clr8 Image::get_pixel8( pixel_pos pos ) const {
		// TODO: code
		return {};
	}

	void Image::put_pixel16( pixel_pos pos, const Clr16 &color ) {
		// TODO: code
	}

	const Clr16 &Image::get_pixel16( pixel_pos pos ) const {
		// TODO: code
		return {};
	}

}
