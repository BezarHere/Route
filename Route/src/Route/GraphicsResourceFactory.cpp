#include "pch.h"
#include "GraphicsResourceFactory.h"
#include "Renderer.h"
#include "../gapi.h"
#include "internal/storage_buffer.h"

namespace route
{
  GraphicsResourceFactory::GraphicsResourceFactory( Renderer &renderer )
    : m_renderer{ renderer }, m_device{} {

  }

  void GraphicsResourceFactory::_create_begin() {
    RT_ASSERT_JIC( is_locked() );
    m_error = Error::Ok;
  }

  void GraphicsResourceFactory::_register( const GraphicsResource &res, RID rid ) {
    (void)res;
    m_resources.push_back( rid );
  }

  RID GraphicsResourceFactory::create_buffer( StorageBufType type ) {
#ifdef GAPI_GL
    GLuint id = 0;
    glGenBuffers( _rt::storage_buffer::to_gl_type( type ), &id );
    return  ResourceServer<StorageBuffer>::add_resource( StorageBuffer( (StorageBufferID)id ) );;
#endif
  }

  RID GraphicsResourceFactory::create_texture( const TextureInfo &info ) {
    // TODO: stuff
    return RIDInvalid;
  }

  bool GraphicsResourceFactory::is_locked() const {
    return m_renderer.m_lock;
  }

}
