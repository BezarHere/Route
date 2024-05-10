#pragma once
#include "ResourceServer.h"
#include "Renderer.h"
#include "StorageBuffer.h"
#include "Texture.h"
#include "Pipeline.h"
#include "GraphicsProfile.h"

namespace route
{
  using GraphicsDeviceID = vpid_t;

  class Renderer;

  class GraphicsDevice
  {
    friend Renderer;
  public:
    enum GraphicsDeviceFlags
    {
      eFlag_None = 0x0000,
      eFlag_Active = 0x0001,
    };

    GraphicsDevice(Window &window);
    ~GraphicsDevice();

    resource<StorageBuffer> create_buffer(StorageBufType type, size_t size);
    resource<Texture> create_texture(const TextureInfo &info);
    resource<Shader> create_shader(const char *source, ShaderType type);
    resource<Pipeline> create_pipeline(const PipelineCreateInfo &info);

    inline bool is_active() const {
      return m_flags & GraphicsDeviceFlags::eFlag_Active;
    }

    inline Renderer &get_renderer() {
      return m_renderer;
    }

    inline const Renderer &get_renderer() const {
      return m_renderer;
    }

    Error update_buffer(StorageBuffer &buffer, const void *data, size_t length, size_t offset) const;

    void _queue_free_buffer(const StorageBuffer &buffer);
    void _queue_free_texture(const Texture &texture);
    void _queue_free_shader(const Shader &shader);
    void _queue_free_pipeline(const Pipeline &pipeline);

  private:
    using StorageBufQueueEntry = StorageBufferID;
    using TextureQueueEntry = TextureID;
    using ShaderQueueEntry = ShaderID;
    using PipelineQueueEntry = PipelineID;

    struct DestroyQueueCollection
    {
      vector<StorageBufQueueEntry> buffers;
      vector<TextureQueueEntry> textures;
      vector<ShaderQueueEntry> shaders;
      vector<PipelineQueueEntry> pipelines;
    };

    void _begin();
    void _end();

    void _free_buffer(const StorageBufQueueEntry &entry);
    void _free_texture(const TextureQueueEntry &entry);
    void _free_shader(const ShaderQueueEntry &entry);
    void _free_pipeline(const PipelineQueueEntry &entry);

    void _process_destroy_queues();

    template <typename _Ty, typename _Ey, typename... _Args>
    inline resource<_Ty> _create_resource(_Args &&...args) const {
      _Ey *ptr = ResourceServer<_Ty>::_allocate(sizeof(_Ey));
      new (ptr) _Ey(args...);
      return resource<_Ty>(ptr);
    }

  private:
    GraphicsDeviceID m_id;
    Window &m_window;
    Renderer m_renderer;

    GraphicsDeviceFlags m_flags;
    DestroyQueueCollection m_destroy_queues;
  };


}
