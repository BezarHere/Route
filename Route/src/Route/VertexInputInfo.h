#pragma once
#include "Resource.h"
#include "Vertex.h"
#include "StaticSpan.h"

namespace route
{
  enum class VertexInputType : uint32_t
  {
    Byte,
    UnsignedByte,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,

    NormalizedByte,
    NormalizedUnsignedByte,
    NormalizedShort,
    NormalizedUnsignedShort,
    NormalizedInt,
    NormalizedUnsignedInt,

    HalfFloat,
    Float,
    Double
  };

  enum class VertexIndexType : uint16_t
  {
    Byte = VertexInputType::Byte,
    Short = VertexInputType::Short,
    Int = VertexInputType::Int,
    UnsignedByte = VertexInputType::UnsignedByte,
    UnsignedShort = VertexInputType::UnsignedShort,
    UnsignedInt = VertexInputType::UnsignedInt,
  };

  enum class VertexInputSize : uint8_t
  {
    One = 1,
    R = 1,

    Two = 2,
    Vec2 = 2,
    RG = 2,

    Three = 3,
    Vec3 = 3,
    RGB = 3,

    Four = 4,
    Vec4 = 4,
    RGBA = 4
  };


  struct VInputAttribute
  {
    VertexInputType type;
    VertexInputSize size;
  };


  struct VertexInputInfo
  {
  public:
    // 24 seems reasonable
    static constexpr size_t MaxVertexAttributes = 24ULL;

    static constexpr int NormalizedVInputTypeBit = 0x10000;
    using container_type = StaticSpan<VInputAttribute, MaxVertexAttributes>;


    VertexInputInfo(uint32_t offset, uint32_t stride, const container_type &attributes);
    VertexInputInfo(const container_type &attributes);
    VertexInputInfo();
    ~VertexInputInfo();

    VertexInputInfo(const VertexInputInfo &copy);
    VertexInputInfo(VertexInputInfo &&move) noexcept;
    VertexInputInfo &operator=(const VertexInputInfo &copy);
    VertexInputInfo &operator=(VertexInputInfo &&move) noexcept;

    /// @param stride a stride of zero will be defaulted to the vertex size.
    ///	              it's always better to set the stride either zero or greater/equal to the vertex size 
    void set_stride(uint32_t stride);
    void set_offset(uint32_t offset);

    inline container_type &get_attrs() {
      return m_container;
    }

    inline const container_type &get_attrs() const {
      return m_container;
    }

    inline uint32_t get_stride() const {
      return m_stride;
    }

    inline uint32_t get_offset() const {
      return m_offset;
    }

    size_t get_vertex_size() const;

  private:
    void _validate_attributes();

  private:
    uint32_t m_offset;
    uint32_t m_stride;
    container_type m_container;
  };
}
