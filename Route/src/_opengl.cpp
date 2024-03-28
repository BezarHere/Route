#include "pch.h"
#include "_opengl.h"
#include <SDL_opengl.h>

errno_t OpenGL::init() {
	if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
	{
		std::cerr << "ERROR: Couldn't init SDL2: " << SDL_GetError() << '\n';
		return EFAULT;
	}

	if (SDL_GL_LoadLibrary( nullptr ))
	{
		std::cerr << "ERROR: Couldn't load OpenGL Lib: " << SDL_GetError() << '\n';
		return EFAULT;
	}




	return 0;
}

void OpenGL::close() {
	SDL_GL_UnloadLibrary();
	SDL_Quit();
}

SDL_GLContext OpenGL::create_context( SDL_Window *window ) {
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, ContextVersion.major );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, ContextVersion.minor );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );
	SDL_GLContext context = SDL_GL_CreateContext( window );
	glewExperimental = true;

	if (!glBindVertexArray)
	{
		auto err = glewInit();
		if (err)
		{
			std::cerr << "ERROR: Couldn't init glew: " << glewGetErrorString( err ) << '\n';
			return nullptr;
		}
	}
	return context;
}
