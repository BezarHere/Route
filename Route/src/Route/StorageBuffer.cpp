#include "pch.h"
#include "../pch.h"
#include "internal/storage_buffer.h"
#include "StorageBuffer.h"
#include "Logger.h"
#include "IMap.h"
#include "Application.h"
#include "GraphicsFactory.h"

namespace route
{


  StorageBuffer::StorageBuffer( StorageBufferID id, StorageBufType type, size_t size, factory &factory )
    : GraphicsResource( factory ), m_id{ id }, m_type{ type }, m_size{ size } {
  }

  StorageBuffer::StorageBuffer( StorageBuffer &&move ) noexcept
    : GraphicsResource( move.get_factory() ), m_id{ move.m_id }, m_type{ move.m_type }, m_size{ move.m_size } {
    move.m_id = StorageBufferID();
  }


  StorageBuffer &StorageBuffer::operator=( StorageBuffer &&move ) noexcept {
    get_factory()._queue_free_buffer( *this );

    m_id = move.m_id;
    m_size = move.m_size;
    m_type = move.m_type;

    move.m_id = StorageBufferID();
    return *this;
  }

  StorageBuffer::~StorageBuffer() noexcept {
    get_factory()._queue_free_buffer( *this );
  }

  Error StorageBuffer::update( uint8_t *data, size_t length, size_t offset ) {


    if (get_factory().is_locked())
      return Error::ServiceLocked;

#ifdef GAPI_GL
    GAPI_IF_GL( Application::graphics_api() == GraphicsAPI::OpenGL ) {
      GL_CALL_POST( glBufferSubData( _rt::storage_buffer::to_gl_type( m_type ), offset, length, data ), return glErrRT( err ) );
      return Error::Ok;
    }
#endif // GAPI_GL


    //return Error::Ok;
  }

}
