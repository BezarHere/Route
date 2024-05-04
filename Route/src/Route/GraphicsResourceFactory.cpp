#include "pch.h"
#include "GraphicsResourceFactory.h"
#include "GraphicsProfile.h"
#include "Renderer.h"
#include "../gapi.h"
#include "internal/storage_buffer.h"

namespace route
{

  RID GraphicsResourceFactory::create_buffer( StorageBufType type, size_t size, int8_t *data ) {
    if (is_locked())
      return RIDInvalid;
#ifdef GAPI_GL
    GLuint id = 0;
    glGenBuffers( _rt::storage_buffer::to_gl_type( type ), &id );
    return  ResourceServer<StorageBuffer>::add_resource( StorageBuffer( (StorageBufferID)id, type, size, *this ) );;
#endif
  }

  RID GraphicsResourceFactory::create_texture( const TextureInfo &info ) {
    // TODO: stuff
    return RIDInvalid;
  }

  RID GraphicsResourceFactory::create_shader( const char *source, ShaderType type ) {
    if (is_locked())
      return RIDInvalid;
#ifdef GAPI_GL
    return  ResourceServer<Shader>::add_resource( Shader( source, type, *this ) );
#endif
    return RID();
  }


  void GraphicsResourceFactory::_queue_free_buffer( const StorageBuffer &buffer ) {
    if (is_locked())
    {
      m_destroy_queues.buffers.push_back( { buffer.get_id() } );
      return;
    }
    _free_buffer( { buffer.get_id() } );
  }

  void GraphicsResourceFactory::_queue_free_texture( const Texture &texture ) {
    if (is_locked())
    {
      m_destroy_queues.textures.push_back( { texture.get_id() } );
      return;
    }
    _free_texture( { texture.get_id() } );
  }

  void GraphicsResourceFactory::_queue_free_shader( const Shader &shader ) {
    if (is_locked())
    {
      m_destroy_queues.shaders.push_back( { shader.get_id() } );
      return;
    }
    _free_shader( { shader.get_id() } );
  }

  void GraphicsResourceFactory::_queue_free_shader_prog( const Pipeline &shader_prog ) {
    if (is_locked())
    {
      m_destroy_queues.shader_programs.push_back( { shader_prog.get_id() } );
      return;
    }
    _free_shader_program( { shader_prog.get_id() } );
  }

  GraphicsResourceFactory::GraphicsResourceFactory( Renderer &renderer )
    : m_renderer{ renderer }, m_device{} {

  }

  void GraphicsResourceFactory::_free_buffer( const StorageBufQueueEntry &entry ) {
#ifdef GAPI_GL
    GAPI_IF_GL( GraphicsProfile::graphics_api() ) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteBuffers( 1, &id );
    }
#endif // GAPI_GL
  }

  void GraphicsResourceFactory::_free_texture( const TextureQueueEntry &entry ) {
#ifdef GAPI_GL
    GAPI_IF_GL( GraphicsProfile::graphics_api() ) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteTextures( 1, &id );
    }
#endif // GAPI_GL
  }

  void GraphicsResourceFactory::_free_shader( const ShaderQueueEntry &entry ) {
#ifdef GAPI_GL
    GAPI_IF_GL( GraphicsProfile::graphics_api() ) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteShader( id );
    }
#endif // GAPI_GL
  }

  void GraphicsResourceFactory::_free_shader_program( const ShaderProgQueueEntry &entry ) {
#ifdef GAPI_GL
    GAPI_IF_GL( GraphicsProfile::graphics_api() ) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteProgram( id );
    }
#endif // GAPI_GL
  }

  void GraphicsResourceFactory::_unlocked() {
    while (!m_destroy_queues.buffers.empty())
    {
      _free_buffer( m_destroy_queues.buffers.back() );
      m_destroy_queues.buffers.pop_back();
    }

    while (!m_destroy_queues.textures.empty())
    {
      _free_texture( m_destroy_queues.textures.back() );
      m_destroy_queues.textures.pop_back();
    }

    while (!m_destroy_queues.shaders.empty())
    {
      _free_shader( m_destroy_queues.shaders.back() );
      m_destroy_queues.shaders.pop_back();
    }

    while (!m_destroy_queues.shader_programs.empty())
    {
      _free_shader_program( m_destroy_queues.shader_programs.back() );
      m_destroy_queues.shader_programs.pop_back();
    }
  }

  bool GraphicsResourceFactory::is_locked() const {
    return m_renderer.is_locked();
  }

}
