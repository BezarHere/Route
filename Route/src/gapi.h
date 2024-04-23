/*
* SHOULD NOT BE INCLUDED
* INTERNAL USE ONLY
*/
#pragma once

#define GAPI_GL
//#define GAPI_VK
//#define GAPI_DX



#ifdef GAPI_GL // VVVV OPENGL

#define GLEW_STATIC
#include <GL/glew.h>

#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_opengl.h>

#define GL_CALL_CHECKS

#ifdef GL_CALL_CHECKS
#define GL_CALL(call) \
{ call; GLenum err = glGetError(); if (err) { char msg[1024]{}; sprintf_s(msg, "GL::Error calling '" #call "', code=%X", err); Logger::write( msg, LogLevel::Error ); } }
#define GL_CALL_RET(call, ret) \
{ call; GLenum err = glGetError();\
if (err) { char msg[1024]{}; sprintf_s(msg, "GL::Error calling '" #call "', code=%X; returning " #ret, err); Logger::write( msg, LogLevel::Error ); return ret; }\
}
#else 
#define GL_CALL(call) (void)(call)
#define GL_CALL_RET(call, ret) (void)(call)
#endif

#elif defined(GAPI_VK) // ^^^^ OPENGL / VVVV VULKAN

#include <vulkan/vulkan.hpp>

#endif // ^^^^ VULKAN
