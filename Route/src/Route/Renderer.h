#pragma once
#include "Scene.h"
#include "Window.h"
#include "Pipeline.h"
#include "RenderCommandQueue.h"


namespace route
{
  class Application;
  class GraphicsDevice;

  struct RendererState
  {
    array<StorageBufferID, (int)StorageBufType::_Max> Buffers{};
    PipelineID Pipeline{};
    PrimitiveTopology topology;
  };

  typedef uint64_t RendererLock;
  typedef void *RendererContext;

  class Renderer
  {
    friend GraphicsDevice;
    friend Application;
  public:
    // 8mb worth of command instances
    static constexpr size_t DefaultCmdBufCapacity = 8388608ULL / sizeof(CommandInstance);
    ~Renderer();

    void render(const Application &app);

    inline GraphicsDevice &get_device() {
      return m_device;
    }

    inline const GraphicsDevice &get_device() const {
      return m_device;
    }

    void set_buffer(const StorageBuffer &buffer);
    void set_pipeline(const Pipeline &shader);

    inline StorageBufferID get_buffer(StorageBufType type) const {
      return m_state.Buffers[static_cast<size_t>(type)];
    }

    inline PipelineID get_pipeline() const {
      return m_state.Pipeline;
    }

  private:
    Renderer(GraphicsDevice &device);
    Renderer(const Renderer &) = delete;
    Renderer &operator =(const Renderer &) = delete;

    void _initialize();

    void _bind_buffer(StorageBufType type) const;
    void _bind_shader_program() const;

    void _process_cmds();
    void _do_command(const CommandInstance &command);

  private:
    class APIState;
    GraphicsDevice &m_device;
    Window &m_window;

    RendererState m_state;

    vector<CommandInstance> m_commands;

    RendererContext m_context;
    APIState *m_api_state;

  };

}
