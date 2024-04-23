#include "pch.h"
#include "_opengl.h"
#include <SDL_opengl.h>
#include "Route/Performance.h"

#ifdef GAPI_GL

LPSDLWindow OpenGL::s_current_window = nullptr;

errno_t OpenGL::init() {
	if (SDL_InitSubSystem( SDL_INIT_VIDEO ) < 0)
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

#endif
