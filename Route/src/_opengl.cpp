#include "pch.h"
#include "_opengl.h"
#include <SDL_opengl.h>

errno_t OpenGL::init() {
	if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
	{
		std::cout << "ERROR: Couldn't init SDL2: " << SDL_GetError() << '\n';
		return EFAULT;
	}

	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 32 );
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 32 );


	return 0;
}

void OpenGL::close() {
	SDL_Quit();
}
