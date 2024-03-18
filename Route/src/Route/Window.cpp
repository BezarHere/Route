#include "pch.h"
#include "../_opengl.h"
#include "Window.h"

static inline void setup_opengl() {
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 1 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, SDL_TRUE );
	SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 24 );
	SDL_GL_SetAttribute( SDL_GL_STENCIL_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, OpenGL::ContextVersion.minor );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, OpenGL::ContextVersion.major );
}

static inline void kill_window_handle( WindowHandle handle ) {
	SDL_DestroyWindow( (SDL_Window *)handle );
}

namespace route
{

	Window::Window( const std::string &title, const Rect2i &frame, WindowFlags flags )
		: m_should_close{ false } {
		setup_opengl();



		m_handle = SDL_CreateWindow(
			title.c_str(),
			frame.x,
			frame.y,
			frame.w,
			frame.h,
			static_cast<Uint32>(flags) | SDL_WINDOW_OPENGL
		);

		if (m_handle == nullptr)
		{
			std::cout << "ERROR: failed to create window, " << SDL_GetError() << '\n';
			throw std::runtime_error( "window" );
		}
	}

	Window::~Window() {
		kill_window_handle( m_handle );
	}

	Window::Window( Window &move ) : m_handle{ move.m_handle } {
		move.m_handle = nullptr;
	}

	Window &Window::operator=( Window &move ) {
		kill_window_handle( m_handle );

		m_handle = move.m_handle;
		move.m_handle = nullptr;
		return *this;
	}
	void Window::poll() {
		SDL_Event event;
		while (SDL_PollEvent( &event ))
		{
			std::cout << "got event type " << event.type << '\n';
			switch (event.type)
			{
			case SDL_QUIT:
				m_should_close = true;
				return;
			default:
				break;
			}
		}

	}
}
