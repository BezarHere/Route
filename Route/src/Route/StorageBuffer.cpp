#include "pch.h"
#include "../pch.h"
#include "internal/gl_utils.h"
#include "StorageBuffer.h"
#include "Logger.h"
#include "IMap.h"
#include "Application.h"
#include "GraphicsDevice.h"

namespace route
{
  StorageBuffer::StorageBuffer(StorageBufferID id, StorageBufType type, size_t size, device &device)
    : GraphicsResource(device), m_id{ id }, m_type{ type }, m_size{ size } {
  }

  StorageBuffer::StorageBuffer(StorageBuffer &&move) noexcept
    : GraphicsResource(move.get_device()), m_id{ move.m_id }, m_type{ move.m_type }, m_size{ move.m_size } {
    move.m_id = StorageBufferID();
  }


  StorageBuffer &StorageBuffer::operator=(StorageBuffer &&move) noexcept {
    get_device()._queue_free_buffer(*this);

    m_id = move.m_id;
    m_size = move.m_size;
    m_type = move.m_type;

    move.m_id = StorageBufferID();
    return *this;
  }

  StorageBuffer::~StorageBuffer() noexcept {
    get_device()._queue_free_buffer(*this);
  }

  Error StorageBuffer::update(const void *data, size_t length, size_t offset) {
    return get_device().update_buffer(*this, data, length, offset);
  }

}
