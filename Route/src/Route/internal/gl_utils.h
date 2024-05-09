#pragma once
#include "../Enums.h"
#include "../StorageBuffer.h"
#include "../VertexInputInfo.h"
#include "../../gapi.h"

#ifdef GAPI_GL

namespace gl_rt
{
  static inline constexpr size_t sizeof_vertex_input_type(const VertexInputType type) noexcept {
    switch (type)
    {
    case VertexInputType::Byte:
    case VertexInputType::NormalizedByte:
      return sizeof(int8_t);
    case VertexInputType::UnsignedByte:
    case VertexInputType::NormalizedUnsignedByte:
      return sizeof(uint8_t);
    case VertexInputType::Short:
    case VertexInputType::NormalizedShort:
      return sizeof(int16_t);
    case VertexInputType::UnsignedShort:
    case VertexInputType::NormalizedUnsignedShort:
      return sizeof(uint16_t);
    case VertexInputType::Int:
    case VertexInputType::NormalizedInt:
      return sizeof(int32_t);
    case VertexInputType::UnsignedInt:
    case VertexInputType::NormalizedUnsignedInt:
      return sizeof(uint32_t);
    case VertexInputType::HalfFloat:
      return sizeof(uint16_t);
    case VertexInputType::Float:
      return sizeof(float);
    case VertexInputType::Double:
      return sizeof(double);
    default:
      return sizeof(float);
    }
  }
  static inline constexpr GLenum get_vertex_type(const VertexInputType type) noexcept {
    switch (type)
    {
    case VertexInputType::Byte:
    case VertexInputType::NormalizedByte:
      return GL_BYTE;
    case VertexInputType::UnsignedByte:
    case VertexInputType::NormalizedUnsignedByte:
      return GL_UNSIGNED_BYTE;
    case VertexInputType::Short:
    case VertexInputType::NormalizedShort:
      return GL_SHORT;
    case VertexInputType::UnsignedShort:
    case VertexInputType::NormalizedUnsignedShort:
      return GL_UNSIGNED_SHORT;
    case VertexInputType::Int:
    case VertexInputType::NormalizedInt:
      return GL_INT;
    case VertexInputType::UnsignedInt:
    case VertexInputType::NormalizedUnsignedInt:
      return GL_UNSIGNED_INT;
    case VertexInputType::HalfFloat:
      return GL_HALF_FLOAT;
    case VertexInputType::Float:
      return GL_FLOAT;
    case VertexInputType::Double:
      return GL_DOUBLE;
    default:
      return GL_FLOAT;
    }
  }

  static inline constexpr bool is_normalized(const VertexInputType type) noexcept {
    switch (type)
    {
    case VertexInputType::NormalizedByte:
    case VertexInputType::NormalizedShort:
    case VertexInputType::NormalizedInt:
    case VertexInputType::NormalizedUnsignedByte:
    case VertexInputType::NormalizedUnsignedShort:
    case VertexInputType::NormalizedUnsignedInt:
      return true;
    default:
      return false;
    }
  }

  static inline constexpr GLuint to_gl_primitive(route::PrimitiveTopology topology) noexcept {
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

  /// @note ONLY FOR ID-INLINED TYPES (e.g. type where vpid_t is a 'number' not a 'pointer')
  static inline constexpr GLuint vpid_gl_name(route::vpid_t id) noexcept {
    return GLuint(id);
  }

  static inline constexpr GLuint get_buffer_type(route::StorageBufType type) {
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
  static inline constexpr GLuint get_buffer_type_binding(route::StorageBufType type) {
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

}
#endif
