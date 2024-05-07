#pragma once
#include "ResourceServer.h"
#include "Renderer.h"
#include "StorageBuffer.h"
#include "Texture.h"
#include "Pipeline.h"

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

    resource_ref<StorageBuffer> create_buffer(StorageBufType type, size_t size, int8_t *data = nullptr);
    resource_ref<Texture> create_texture(const TextureInfo &info);
    resource_ref<Shader> create_shader(const char *source, ShaderType type);

    inline bool is_active() const {
      return flags & GraphicsDeviceFlags::eFlag_Active;
    }

    inline Renderer &get_renderer() {
      return m_renderer;
    }

    inline const Renderer &get_renderer() const {
      return m_renderer;
    }

    void _queue_free_buffer(const StorageBuffer &buffer);
    void _queue_free_texture(const Texture &texture);
    void _queue_free_shader(const Shader &shader);
    void _queue_free_shader_prog(const Pipeline &shader_prog);

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

    void _free_buffer(const StorageBufQueueEntry &entry);
    void _free_texture(const TextureQueueEntry &entry);
    void _free_shader(const ShaderQueueEntry &entry);
    void _free_shader_program(const ShaderProgQueueEntry &entry);

    void _unlocked();

    template <typename _Ty, typename _Ey, typename... _Args>
    inline resource_ref<_Ty> _create_resource(_Args &&...args) const {
      _Ey *ptr = ResourceServer<_Ty>::_allocate(sizeof(_Ey));
      new (ptr) _Ey(args...);
      return resource_ref<_Ty>(ptr);
    }

  private:
    GraphicsDeviceID m_id;
    Window &m_window;
    Renderer m_renderer;

    GraphicsDeviceFlags flags;
    DestroyQueueCollection m_destroy_queues;
  };


}
