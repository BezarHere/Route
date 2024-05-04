/*
* SHOULD NOT BE INCLUDED
* INTERNAL USE ONLY
*/
#pragma once

#include <array>
#include <exception>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

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

template <typename T>
static inline void _format_join( std::ostringstream &stream, const T &value ) {
  stream << value;
}

template <typename T1, typename T2, typename... VArgs>
static inline void _format_join( std::ostringstream &stream, const T1 &value1, const T2 &value2, const VArgs &... args ) {
  stream << value1;
  return _format_join( stream, value2, args... );
}

template <typename... VArgs>
static inline std::string format_join( const VArgs &... args ) {
  std::ostringstream ss{ 256 };
  _format_join( ss, args... );
  return ss.str();
}

