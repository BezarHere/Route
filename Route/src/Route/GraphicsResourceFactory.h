#pragma once
#include "ResourceServer.h"
#include "StorageBuffer.h"
#include "Texture.h"
#include "ShaderProgram.h"

namespace route
{
  using DeviceID = int64_t;
  typedef class GraphicsResourceFactory GphxResFac;

  class Renderer;

  class GraphicsResourceFactory
  {
    friend Renderer;
  public:
    RID create_buffer( StorageBufType type, size_t size, int8_t *data = nullptr );
    RID create_texture( const TextureInfo &info );
    RID create_shader( const char *source, ShaderType type );

    inline Error error() const {
      return m_error;
    }

    bool is_locked() const;

    inline Renderer &get_renderer() const {
      return m_renderer;
    }

    void _queue_free_buffer( const StorageBuffer &buffer );
    void _queue_free_texture( const Texture &texture );
    void _queue_free_shader( const Shader &shader );
    void _queue_free_shader_prog( const Pipeline &shader_prog );

  private:
    using StorageBufQueueEntry = StorageBufferID;
    using TextureQueueEntry = TextureID;
    using ShaderQueueEntry = ShaderID;
    using ShaderProgQueueEntry = PipelineID;

    struct DestroyQueueCollection
    {
      vector<StorageBufQueueEntry> buffers;
      vector<TextureQueueEntry> textures;
      vector<ShaderQueueEntry> shaders;
      vector<ShaderProgQueueEntry> shader_programs;
    };

    GraphicsResourceFactory( Renderer &renderer );
    void _free_buffer( const StorageBufQueueEntry &entry );
    void _free_texture( const TextureQueueEntry &entry );
    void _free_shader( const ShaderQueueEntry &entry );
    void _free_shader_program( const ShaderProgQueueEntry &entry );

    void _unlocked();

  private:
    Renderer &m_renderer;
    DeviceID m_device;
    Error m_error;
    DestroyQueueCollection m_destroy_queues;
  };


}
