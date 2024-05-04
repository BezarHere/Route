#pragma once
#include "../StorageBuffer.h"
#include "../../gapi.h"

namespace _rt
{

#ifdef GAPI_GL
  static inline constexpr GLuint to_gl_name( route::vpid_t id ) {
    return (GLuint)id;
  }
#endif

  struct storage_buffer
  {
#ifdef GAPI_GL
    static inline constexpr GLuint to_gl_type( route::StorageBufType type ) {
      constexpr std::array<GLuint, (size_t)route::StorageBufType::_Max> types =
      {
        GL_VERTEX_ARRAY,
        GL_ELEMENT_ARRAY_BUFFER,
        GL_TEXTURE_BUFFER,
        GL_UNIFORM_BUFFER,
        GL_SHADER_STORAGE_BUFFER,
        GL_TRANSFORM_FEEDBACK_BUFFER,
        GL_DISPATCH_INDIRECT_BUFFER,
        GL_COPY_READ_BUFFER,
        GL_COPY_WRITE_BUFFER,
        GL_QUERY_BUFFER
      };
      static_assert(types[ types.size() - 1 ], "Types array has an insufficient number of defined elements");
      return types[ (size_t)type ];
    }

#endif // GAPI_GL

  };
}
