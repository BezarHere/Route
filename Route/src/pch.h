#pragma once

#include <array>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#define VERBOSE

#include "gapi.h"
#include "_opengl.h"



namespace route
{
}

using namespace route;

#define SDL_FNCHECK(call) \
{ int err = (call); if (err) { char buf[256]{}; sprintf_s(buf, std::size(buf), "Failed to call " #call " at " __FILE__ ":%u, returned %d", __LINE__, err); Logger::write( buf, LogLevel::Error ); } }

#define SDL_FNCHECK_V(call, statment) \
{ int err = (call); \
if (err) \
	{ char buf[256]{}; sprintf_s(buf, std::size(buf), "Failed to call " #call " at " __FILE__ ":%u, returned %d", __LINE__, err); Logger::write( buf, LogLevel::Error ); statment; }\
}
