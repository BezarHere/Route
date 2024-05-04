#pragma once
#include "Resource.h"
#include "Vertex.h"

namespace route
{



  enum class VertexInputType : uint32_t
  {
    Byte = 0x1400,
    UnsignedByte,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,

    NormalizedByte = Byte | 0x10000,
    NormalizedUnsignedByte = UnsignedByte | 0x10000,
    NormalizedShort = Short | 0x10000,
    NormalizedUnsignedShort = UnsignedShort | 0x10000,
    NormalizedInt = Int | 0x10000,
    NormalizedUnsignedInt = UnsignedInt | 0x10000,

    HalfFloat = 0x140B,
    Float = 0x1406,
    Double = 0x140A
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
    Single = 1,

    Double = 2,
    Vec2 = 2,

    Trible = 3,
    RGB = 3,
    Vec3 = 3,

    Qoudruble = 4,
    Vec4 = 4,
    RGBA = 4,
    Color = 4
  };


  struct VInputAttribute
  {
    VertexInputType type;
    VertexInputSize size;
  };

  typedef unsigned VInputDescID;

  struct VertexInputState
  {
  public:
    static constexpr int NormalizedVInputTypeBit = 0x10000;
    using container_type = vector<VInputAttribute>;

    // 32 seems reasonable
    static constexpr size_t MaxVertexAttributes = 32ULL;

    VertexInputState();
    ~VertexInputState();

    VertexInputState( const VertexInputState &copy );
    VertexInputState( VertexInputState &&move ) noexcept;
    VertexInputState &operator=( const VertexInputState &copy );
    VertexInputState &operator=( VertexInputState &&move ) noexcept;

    /// @param stride a stride of zero will be defaulted to the vertex size.
    ///	              it's always better to set the stride either zero or greater/equal to the vertex size 
    void set_stride( uint32_t stride );
    void set_offset( uint32_t offset );

    inline VInputDescID get_name() const {
      return m_id;
    }

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
    VInputDescID m_id;
    container_type m_container;
    uint32_t m_offset;
    uint32_t m_stride;
  };
}
