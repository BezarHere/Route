#pragma once
#include "Scene.h"
#include "Window.h"
#include "RenderCommandQueue.h"
#include "GraphicsResourceFactory.h"
#include "ShaderProgram.h"


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
    ShaderProgramID ShaderProg{};
  };

  typedef uint64_t RendererLock;
  typedef void *RendererContext;

  class Renderer
  {
    friend GphxResFac;
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
    void set_shader_program( const ShaderProgram &shader );

    inline StorageBufferID get_bound_buffer( StorageBufType type ) const {
      return m_state.Buffers[ static_cast<size_t>(type) ];
    }

    inline ShaderProgramID get_bound_shader_prog() const {
      return m_state.ShaderProg;
    }

  private:
    Renderer( const Renderer & ) = delete;
    Renderer &operator =( const Renderer & ) = delete;

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
