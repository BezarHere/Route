#pragma once
#include "Scene.h"
#include "Window.h"
#include "RenderCommandQueue.h"
#include "GraphicsFactory.h"


namespace route
{
  class Application;



  class RenderCommandQueue
  {
  public:
    using command_boxed = rcq::command_boxed;


    vector<command_boxed> commands;
  };

  struct RendererState
  {
    array<StorageBufferID, (int)StorageBufType::_Max> Buffers{};
    PipelineID Pipeline{};
  };

  typedef uint64_t RendererLock;
  typedef void *RendererContext;

  class Renderer
  {
    friend GphxResFac;
    friend Application;
  public:
    Renderer( Window &window );
    ~Renderer();

    void render( const Application &app );

    inline GphxResFac &get_factory() {
      return m_factory;
    }

    inline const GphxResFac &get_factory() const {
      return m_factory;
    }

    void set_buffer( const StorageBuffer &buffer );
    void set_pipeline( const Pipeline &shader );

    inline StorageBufferID get_buffer( StorageBufType type ) const {
      return m_state.Buffers[ static_cast<size_t>(type) ];
    }

    inline PipelineID get_pipeline() const {
      return m_state.Pipeline;
    }

    /// @brief gets weather the renderer is locked, the renderer should lock when it's not active
    /// @brief only unlocked renderers can manipulate GPU resource.
    /// @returns weather the renderer is locked
    bool is_locked() const;

  private:
    Renderer( const Renderer & ) = delete;
    Renderer &operator =( const Renderer & ) = delete;

    void _initialize();

    void _bind_buffer( StorageBufType type) const;
    void _bind_shader_program() const;

    void _do_command_gl( const RenderCommandQueue::command_boxed &command );
    void _do_command_vk( const RenderCommandQueue::command_boxed &command );

  private:
    class APIState;
    RendererLock m_lock;
    Window &m_window;
    RendererState m_state;

    RenderCommandQueue m_command_queue;

    RendererContext m_context;
    APIState *m_api_state;
    // the factory should always be the last member field, so it can be initialized after all other fields
    GphxResFac m_factory;
  };

}
