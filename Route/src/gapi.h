#pragma once

#define GAPI_GL
//#define GAPI_VK
//#define GAPI_DX



#ifdef GAPI_GL
#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>
#endif
