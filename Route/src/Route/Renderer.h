#pragma once
#include "Scene.h"
#include "Window.h"
#include "Pipeline.h"
#include "RenderCommands.h"


namespace route
{
  class Application;
  class GraphicsDevice;

  struct RendererState
  {
    array<StorageBufferID, (int)StorageBufType::_Max> Buffers{};
    resource<Pipeline> Pipeline{};
    PrimitiveTopology topology;
  };

  typedef uint64_t RendererLock;
  typedef void *RendererContext;

  class Renderer
  {
    friend GraphicsDevice;
    friend Application;
  public:
    using commands_container = RenderCollector::container_type;
    ~Renderer();

    void render(const Application &app);

    inline GraphicsDevice &get_device() {
      return m_device;
    }

    inline const GraphicsDevice &get_device() const {
      return m_device;
    }

    void set_buffer(const StorageBuffer &buffer);
    void set_pipeline(const resource<Pipeline> &pipeline);

    inline StorageBufferID get_buffer(StorageBufType type) const {
      return m_state.Buffers[static_cast<size_t>(type)];
    }

    inline const resource<Pipeline> &get_pipeline() const {
      return m_state.Pipeline;
    }

  private:
    Renderer(GraphicsDevice &device);
    Renderer(const Renderer &) = delete;
    Renderer &operator =(const Renderer &) = delete;

    void _initialize();
    void _finalize();

    void _bind_buffer(StorageBufType type) const;
    void _bind_pipeline() const;

    void _process_cmds();
    void _do_command(const CommandInstance &command);

  private:
    class APIState;
    GraphicsDevice &m_device;
    Window &m_window;

    RendererState m_state;

    commands_container *m_commands;
    APIState *m_api_state;

  };

}
