#include "pch.h"
#include "Image.h"
#include <SDL.h>

namespace route
{
	struct Image::State
	{
		SDL_Surface *surface;

		inline ~State() throw() {
			SDL_FreeSurface( surface );
		}
	};

	Image::Image( Vec2u size ) : m_state{ new State() } {
		m_state->surface = SDL_CreateRGBSurfaceWithFormat( 0, size.x, size.y, 32, SDL_PixelFormatEnum::SDL_PIXELFORMAT_RGBA8888 );
	}

	Image::Image( const Image &copy ) {
	}

	Image::~Image() {
		delete m_state;
	}

	Vec2u Image::size() const {
		return Vec2u( m_state->surface->w, m_state->surface->h );
	}

	uint32_t Image::width() const {
		return m_state->surface->w;
	}

	uint32_t Image::height() const {
		return m_state->surface->h;
	}

	uint32_t *Image::at( const Vec2u &position ) {
		return nullptr;
	}

	const uint32_t *Image::at( const Vec2u &position ) const {
		return nullptr;
	}

}
