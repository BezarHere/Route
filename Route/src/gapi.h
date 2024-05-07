/*
* SHOULD NOT BE INCLUDED
* INTERNAL USE ONLY
*/
#pragma once
#include "Route/Error.h"

// you can define:
#define GAPI_GL
//#define GAPI_VK

#if defined(GAPI_GL) && !defined(GAPI_VK)
#define GAPI_GL_ONLY
#elif !defined(GAPI_GL) && defined(GAPI_VK)
#define GAPI_VK_ONLY
#endif

#ifdef GAPI_GL_ONLY
#define GAPI_IF_GL(expr) // no need to check, we only have opengl
#elif defined(GAPI_GL)
#define GAPI_IF_GL(expr) if (expr) // do check, we can be using vulkan
#endif

#ifdef GAPI_VK_ONLY
#define GAPI_IF_VK(expr) // no need to check, we only have vulkan
#elif defined(GAPI_VK)
#define GAPI_IF_VK(expr) if (expr) // do check, we can be using opengl
#endif

#ifdef GAPI_GL // VVVV OPENGL VVVV

#define GLEW_STATIC
#include <GL/glew.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

static inline GLenum glGetLastError() {
  GLenum last = GL_NO_ERROR;
  GLenum err = glGetError();
  while (err != GL_NO_ERROR)
  {
    last = err;
    err = glGetError();
  }
  return last;
}

static constexpr inline route::Error glErrRT( const GLenum glErr ) {
  switch (glErr)
  {
  case GL_NO_ERROR:
    return route::Error::Ok;
  case GL_INVALID_ENUM:
    return route::Error::InvalidEnum;
  case GL_INVALID_OPERATION:
    return route::Error::InvalidOperation;
  case GL_INVALID_VALUE:
    return route::Error::InvalidValue;
  case GL_INVALID_FRAMEBUFFER_OPERATION:
    return route::Error::ObjectIncomplete;
  case GL_OUT_OF_MEMORY:
    return route::Error::OutOfMemory;
  case GL_STACK_OVERFLOW:
    return route::Error::StackOverflow;
  case GL_STACK_UNDERFLOW:
    return route::Error::StackUnderflow;
  default:
    return route::Error::UNKNOWN;
  }
}

#define GL_CALL_CHECKS

#ifdef GL_CALL_CHECKS
#define GL_CALL(call) GL_CALL_POST(call, (void)0)
#define GL_CALL_POST(call, post) \
{ call; GLenum err = glGetError();\
if (err) { char msg[1024]{}; sprintf_s(msg, "GL::Error calling '" #call "', code=%X", err); Logger::write( msg, LogLevel::Error ); post; }\
}
#else 
#define GL_CALL(call) (void)(call)
#define GL_CALL_RET(call, ret) (void)(call)
#endif

#elif defined(GAPI_VK) // ^^^^ OPENGL ^^^^ / VVVV VULKAN VVVV

#include <vulkan/vulkan.hpp>

#endif // ^^^^ VULKAN ^^^^
