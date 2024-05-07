#pragma once
#include "../Enums.h"
#include "../../gapi.h"

namespace _rt
{

#ifdef GAPI_GL
  static inline constexpr GLuint to_gl_name(route::vpid_t id) {
    return GLuint(id);
  }

  static inline constexpr GLuint to_gl_primitive(route::PrimitiveTopology topology) {
    constexpr GLuint Lookup[] = {
      GL_TRIANGLES,
      GL_TRIANGLE_STRIP,
      GL_TRIANGLE_FAN,
      GL_LINES,
      GL_LINE_STRIP,
      GL_POINTS,
      GL_PATCHES,
    };
    return Lookup[static_cast<size_t>(topology)];
  }
#endif

  
}
