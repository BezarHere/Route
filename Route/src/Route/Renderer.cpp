#include "pch.h"
#include "../pch.h"
#include "Logger.h"
#include "Renderer.h"
#include "StorageBuffer.h"
#include "Application.h"

#include "internal/storage_buffer.h"

namespace route
{
  class Renderer::APIState
  {
  public:

    APIState(Renderer &p_renderer) : renderer{ p_renderer } {
      const auto vertex_shader =
        "layout (location = 0) in vec4 pos; out vec3 clr;"
        "void main() { gl_Position = pos; clr = vec3((pos.xy + vec2(1)) / 2.0, 0.5); }"
        ;
      const auto frag_shader =
        "in vec3 clr; layout (location = 0) out vec4 Color;"
        "void main() { Color = vec4(clr, 1.0); }"
        ;
      resource_ref<Shader> vertex_shader_res = renderer.get_factory().create_shader(vertex_shader, ShaderType::Vertex);
      resource_ref<Shader> fragment_shader_res = renderer.get_factory().create_shader(frag_shader, ShaderType::Fragment);

      pipeline = Pipeline({ vertex_shader_res.get(), fragment_shader_res.get() });

    }

    ~APIState() {
    }

    Renderer &renderer;
    Pipeline pipeline;
    Vec2u last_window_size;
  };

  Renderer::Renderer(Window &window)
    : m_window{ window }, m_factory{ *this } {
    // TODO: start OpenGL
    m_context = OpenGL::create_context(static_cast<LPSDLWindow>(m_window.m_handle));
    if (!m_context)
    {
      Logger::write("Failed to create context for Renderer", LogLevel::Error);
      return;
    }
  }

  Renderer::~Renderer() {
    const auto current_context = OpenGL::get_context();
    const auto current_window = OpenGL::get_context_window();

    OpenGL::set_context(m_window.m_handle, m_context);
    delete m_api_state;
    OpenGL::delete_context(m_context);

    OpenGL::set_context(current_window, current_context);
  }

  void Renderer::render(const Application &app) {
    SDL_FNCHECK_V(OpenGL::set_context(m_window.m_handle, m_context), return);

    constexpr Vec2f verts[]
    {
      { 0.0f, 0.5f },
      { 0.0f, 1.0f },
      { 1.0f, 1.0f },
      { 0.0f, 0.0f },
      { -1.0f, 0.0f },
      { -1.0f, -1.0f },
    };

    const auto wnd_sz = m_window.size();
    if (wnd_sz != m_api_state->last_window_size)
      glViewport(-32, -32, wnd_sz.x, wnd_sz.y);

    m_api_state->last_window_size = wnd_sz;
    m_factory._unlocked();

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    GLuint vbo{ 0 };
    GLuint vao{ 0 };

    GL_CALL(glGenVertexArrays(1, &vao));
    GL_CALL(glGenBuffers(1, &vbo));
    GL_CALL(glBindVertexArray(vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * std::size(verts), verts, GL_STATIC_DRAW));

    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vec2f), nullptr));

    set_pipeline(m_api_state->pipeline);
    _bind_shader_program();
    GL_CALL(glBindVertexArray(vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));

    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, std::size(verts)));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    m_state.Pipeline = 0;
    _bind_shader_program();

    GL_CALL(glDeleteBuffers(1, &vbo));
    GL_CALL(glDeleteVertexArrays(1, &vao));

    SDL_GL_SwapWindow(static_cast<LPSDLWindow>(m_window.m_handle));
  }

  void Renderer::set_buffer(const StorageBuffer &buffer) {
    m_state.Buffers[static_cast<size_t>(buffer.get_type())] = buffer.get_id();
  }

  void Renderer::set_pipeline(const Pipeline &shader) {
    m_state.Pipeline = shader.get_id();
  }

  bool Renderer::is_locked() const {
    return m_lock;
  }

  void Renderer::_initialize() {
    const auto current_context = OpenGL::get_context();
    const auto current_window = OpenGL::get_context_window();

    SDL_FNCHECK_V(OpenGL::set_context(m_window.m_handle, m_context), return);

    m_api_state = new APIState{ *this };
    m_window._initialize();

    if (current_context && current_window)
      SDL_FNCHECK_V(OpenGL::set_context(current_window, current_context), return);
  }

  void Renderer::_bind_buffer(const StorageBufType type) const {
#ifdef GAPI_GL
    GAPI_IF_GL(Application::graphics_api() == GraphicsAPI::OpenGL) {
      glBindBuffer(_rt::storage_buffer::to_gl_type(type), _rt::to_gl_name(m_state.Buffers[static_cast<size_t>(type)]));
      // TODO: check for errors?
    }
#endif // GAPI_GL

  }

  void Renderer::_bind_shader_program() const {
#ifdef GAPI_GL
    GAPI_IF_GL(Application::graphics_api() == GraphicsAPI::OpenGL) {
      glUseProgram(_rt::to_gl_name(m_state.Pipeline));
      // TODO: check for errors?
    }
#endif // GAPI_GL
  }

#ifdef GAPI_GL
  void Renderer::_do_command_gl(const RenderCommandQueue::command_boxed &command) {
    using rcq::CommandType;
    switch (command->type)
    {
    case CommandType::BindVertexSource:
      {
        const auto &c = command.get<rcq::CommandBindVertexSource>();
        const auto &vertex_buf = c.vertex_buffer;
        RT_ASSERT_RELEASE(vertex_buf->get_type() == StorageBufType::Vertex);
      }
    default:
      break;
    }
  }
#endif

#ifdef GAPI_VK
  void Renderer::_do_command_vk(const RenderCommandQueue::command_boxed &command) {
    using rcq::CommandType;
    switch (command->get_type)
    {
    case CommandType::BindVertexSource:
    default:
      break;
    }
      }
#endif

    }
