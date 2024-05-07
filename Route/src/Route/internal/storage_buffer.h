#pragma once
#include "gl_utils.h"
#include "../StorageBuffer.h"


namespace route
{
  struct storage_buffer
  {
#ifdef GAPI_GL
    static inline constexpr GLuint to_gl_type(route::StorageBufType type) {
      constexpr std::array<GLuint, (size_t)route::StorageBufType::_Max> types =
      {
        GL_ARRAY_BUFFER,
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
      static_assert(types[types.size() - 1], "Types array has an insufficient number of defined elements");
      return types[(size_t)type];
    }
    static inline constexpr GLuint to_gl_type_binding(route::StorageBufType type) {
      constexpr std::array<GLuint, (size_t)route::StorageBufType::_Max> types =
      {
        GL_ARRAY_BUFFER_BINDING,
        GL_ELEMENT_ARRAY_BUFFER_BINDING,
        GL_TEXTURE_BUFFER_BINDING,
        GL_UNIFORM_BUFFER_BINDING,
        GL_SHADER_STORAGE_BUFFER_BINDING,
        GL_TRANSFORM_FEEDBACK_BUFFER_BINDING,
        GL_DISPATCH_INDIRECT_BUFFER_BINDING,
        GL_COPY_READ_BUFFER_BINDING,
        GL_COPY_WRITE_BUFFER_BINDING,
        GL_QUERY_BUFFER_BINDING
      };
      static_assert(types[types.size() - 1], "Types array has an insufficient number of defined elements");
      return types[(size_t)type];
    }

#endif // GAPI_GL

  };
}
