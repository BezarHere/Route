/*
* SHOULD NOT BE INCLUDED
* INTERNAL USE ONLY
*/

#pragma once
#include "pch.h"
#include "Route/Window.h"

using LPSDLWindow = SDL_Window *;

#ifdef GAPI_GL
class OpenGL
{
public:
	static constexpr struct {
		unsigned major, minor;
	} ContextVersion = { 4, 0 };

	static errno_t init();
	static void close();
	static inline bool is_glew_loaded() {
		return glBindVertexArray != nullptr;
	}

	static inline SDL_GLContext create_context( SDL_Window *window ) {
		// setup context
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, ContextVersion.major );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, ContextVersion.minor );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		// create the context
		SDL_GLContext context = SDL_GL_CreateContext( window );

		if (!is_glew_loaded())
		{
			// initalize glew
			glewExperimental = true;
			GLenum err = glewInit();

			// error checks
			if (err)
			{
				std::cerr << "ERROR: Couldn't init glew: " << glewGetErrorString( err ) << '\n';
				return nullptr;
			}

		}
		return context;
	}

	static inline SDL_GLContext get_context() {
		return SDL_GL_GetCurrentContext();
	}

	static inline LPSDLWindow get_context_window() {
		return s_current_window;
	}

	static inline int set_context( void *handle, SDL_GLContext context ) {
		const int res = SDL_GL_MakeCurrent( static_cast<LPSDLWindow>(handle), context );
		if (res)
			return res;
		s_current_window = static_cast<LPSDLWindow>(handle);
		return 0;
	}

	static inline void delete_context( SDL_GLContext context ) {
		return SDL_GL_DeleteContext( context );
	}

	static inline GLint query_int( const GLenum id ) {
		GLint value;
		glGetIntegerv( id, &value );
		return value;
	}

	static inline GLfloat query_float( const GLenum id ) {
		GLfloat value;
		glGetFloatv( id, &value );
		return value;
	}

private:
	static LPSDLWindow s_current_window;
};
#endif
