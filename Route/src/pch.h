#pragma once

#include <array>
#include <exception>
#include <iostream>
#include <string>
#include <vector>

#define VERBOSE

#include "gapi.h"
#include "_opengl.h"

#include "Route/Bases.h"

namespace route
{
}

using namespace route;

// static inlined constexpr function defintion prefix
#define RT_FDEF_SICX static inline constexpr
// static force-inlined constexpr function definition prefix
#define RT_FDEF_SFIC static FORCE_INLINE constexpr
// Static Compile Time Only function defintion prefix
#define RT_FDEF_SCTO static inline consteval

// defining this will make the resource manger check the RIDs when getting resources
#define RT_RID_CHECKS

#define SDL_FNCHECK(call) \
{ int err = (call); if (err) { char buf[256]{}; sprintf_s(buf, std::size(buf), "Failed to call " #call " at " __FILE__ ":%u, returned %d", __LINE__, err); Logger::write( buf, LogLevel::Error ); } }

#define SDL_FNCHECK_V(call, statment) \
{ int err = (call); \
if (err) \
	{ char buf[256]{}; sprintf_s(buf, std::size(buf), "Failed to call " #call " at " __FILE__ ":%u, returned %d", __LINE__, err); Logger::write( buf, LogLevel::Error ); statment; }\
}
