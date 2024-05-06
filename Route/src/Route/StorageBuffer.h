#pragma once
#include "Resource.h"

namespace route
{
  enum class StorageBufType
  {
    Invalid = -1,
    Vertex,
    Index,
    Texture,
    UniformBlock,
    ShaderStorage,
    TransformFeedback,
    DrawIndirect,
    CopySource,
    CopyDestination,
    QueryResult,

    _Max,
  };

  typedef vpid_t StorageBufferID;

  class StorageBuffer : public GraphicsResource
  {
    friend GraphicsFactory;
  public:
    StorageBuffer( StorageBuffer && ) noexcept;
    StorageBuffer &operator=( StorageBuffer && ) noexcept;
    ~StorageBuffer() noexcept;

    FORCE_INLINE StorageBufferID get_id() const {
      return m_id;
    }

    FORCE_INLINE StorageBufType get_type() const {
      return m_type;
    }

    FORCE_INLINE size_t get_size() const {
      return m_size;
    }

    Error update( uint8_t *data, size_t length, size_t offset = 0U );

  private:
    // can lead to segfaults if not used carefully
    StorageBuffer( StorageBufferID id, StorageBufType type, size_t size, factory &factory );

  private:
    StorageBufferID m_id;
    StorageBufType m_type;
    size_t m_size;
  };

}
