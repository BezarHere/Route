#pragma once
#define GLEW_STATIC
#define SDL_MAIN_HANDLED
#include <GL/glew.h>
#include <SDL.h>
#include <SDL_opengl.h>

#include <array>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#include "_opengl.h"

#define VERBOSE

namespace route
{
}

using namespace route;

#define SDL_FNCHECK(call) \
{ int err = (call); if (err) { char buf[256]{}; sprintf_s(buf, std::size(buf), "Failed to call " #call " at " __FILE__ ":%u, returned %d", __LINE__, err); Logger::write( buf, LogLevel::Error ); } }
