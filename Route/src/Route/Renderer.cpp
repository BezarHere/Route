#include "pch.h"
#include "../pch.h"
#include "Logger.h"
#include "Renderer.h"
#include "GraphicsDevice.h"
#include "StorageBuffer.h"
#include "Application.h"
#include "GraphicsProfile.h"

#include "internal/gl_utils.h"
#include "internal/pipeline.h"
#include "Pipeline.h"

#ifdef GAPI_GL
inline static void gl_update_vao(const VertexInputInfo &info) {
  const size_t stride = info.get_stride() ? info.get_stride() : info.get_vertex_size();
  for (GLuint i = 0; i < info.get_attrs().size(); i++)
  {
    const auto &attr = info.get_attrs()[i];
    glEnableVertexAttribArray(i);
    glVertexAttribPointer(
      i,
      (GLint)attr.size,
      gl_rt::get_vertex_type(attr.type),
      gl_rt::is_normalized(attr.type),
      stride, (const void *)info.get_offset());
  }
}
#endif

namespace route
{
  struct PrimitivesData
  {
    resource<StorageBuffer> rect_buffer;
    array<CommandInstance, 3> rect_commands = {
      CommandInstance(rcq::CmdType::SetPrimitiveTopology, PrimitiveTopology::TriangleStrips),
      CommandInstance(rcq::CmdType::BindVertexBuffer, rect_buffer),
      CommandInstance(rcq::CmdType::Draw)
    };
  };

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
      resource<Shader> vertex_shader_res = renderer.get_device().create_shader(vertex_shader, ShaderType::Vertex);
      resource<Shader> fragment_shader_res = renderer.get_device().create_shader(frag_shader, ShaderType::Fragment);



      StaticSpan<VInputAttribute, 4> via{};
      via.emplace_back();
      via.back().type = VertexInputType::Float;
      via.back().size = VertexInputSize::Vec2;

      via.emplace_back();
      via.back().type = VertexInputType::NormalizedUnsignedByte;
      via.back().size = VertexInputSize::RGBA;

      via.emplace_back();
      via.back().type = VertexInputType::NormalizedUnsignedShort;
      via.back().size = VertexInputSize::Vec2;

      PipelineCreateInfo info{};
      info.shaders = { vertex_shader_res.get(), fragment_shader_res.get() };
      info.input_states = { via };

      pipeline = renderer.get_device().create_pipeline(info);


      const Vrtx8C2 vertices[6] = {
        { {0.5f, 0.5f}, { 255, 255, 255 }, {} },
        { {-0.5f, 0.5f}, { 0, 255, 255 }, {} },
        { {-0.5f, -0.5f}, { 255, 0, 255 }, {} },
        { {0.5f, 0.5f}, { 255, 255, 0 }, {} },
        { {-0.5f, -0.5f}, { 0, 255, 0 }, {} },
        { {0.5f, -0.5f}, { 255, 0, 0 }, {} }
      };

      primitive_data.rect_buffer = renderer.get_device().create_buffer(StorageBufType::Vertex, sizeof(vertices));
      std::cout << "rect primitive update: " << primitive_data.rect_buffer->update(vertices, sizeof(vertices), 0) << '\n';
    }

    ~APIState() {
    }

    Renderer &renderer;
    resource<Pipeline> pipeline;
    Vec2u last_window_size;
    PrimitivesData primitive_data = {};
#if GAPI_VK
    vkCommandBuffer cmd_buffer;
#endif // GAPI_VK

  };

  Renderer::Renderer(GraphicsDevice &device)
    : m_window{ device.m_window }, m_device{ device }, m_commands{ new commands_container() } {

  }

  Renderer::~Renderer() {
    delete m_commands;
  }

  void Renderer::render(const Application &app) {
    m_device._begin();

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
      glViewport(0, 0, wnd_sz.x, wnd_sz.y);

    m_api_state->last_window_size = wnd_sz;

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    //GLuint vbo{ 0 };
    GLuint vao{ 0 };

    GL_CALL(glGenVertexArrays(1, &vao));
    //GL_CALL(glGenBuffers(1, &vbo));
    //GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, vbo));
    //GL_CALL(glBufferData(GL_ARRAY_BUFFER, sizeof(Vec2f) * std::size(verts), verts, GL_STATIC_DRAW));


    set_pipeline(m_api_state->pipeline);
    _bind_pipeline();
    //GL_CALL(glBindVertexArray(vao));
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, (GLuint)m_api_state->primitive_data.rect_buffer->get_id()));

    GL_CALL(glBindVertexArray(vao));
    GL_CALL(glEnableVertexAttribArray(0));
    GL_CALL(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vrtx8C2), nullptr));

    GL_CALL(glDrawArrays(GL_TRIANGLES, 0, std::size(verts)));

    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    GL_CALL(glBindVertexArray(0));
    //m_state.Pipeline = 0;
    //_bind_pipeline();

    //GL_CALL(glDeleteBuffers(1, &vbo));
    GL_CALL(glDeleteVertexArrays(1, &vao));

    // todo: move to the device
    SDL_GL_SwapWindow(static_cast<LPSDLWindow>(m_window.m_handle));
    m_device._end();
  }

  void Renderer::set_buffer(const StorageBuffer &buffer) {
    m_state.Buffers[static_cast<size_t>(buffer.get_type())] = buffer.get_id();
  }

  void Renderer::set_pipeline(const resource<Pipeline> &pipeline) {
    m_state.Pipeline = pipeline;
  }

  void Renderer::_initialize() {
    m_device._begin();

    m_api_state = new APIState{ *this };
    m_window._initialize();

    m_device._end();
  }

  void Renderer::_finalize() {
    m_device._begin();
    delete m_api_state;
    m_device._end();
  }

  void Renderer::_bind_buffer(const StorageBufType type) const {
#ifdef GAPI_GL
    GAPI_IF_GL(Application::graphics_api() == GraphicsAPI::OpenGL) {
      glBindBuffer(gl_rt::get_buffer_type(type), gl_rt::vpid_gl_name(m_state.Buffers[static_cast<size_t>(type)]));
      // TODO: check for errors?
    }
#endif // GAPI_GL

  }

  void Renderer::_bind_pipeline() const {
    if (!m_state.Pipeline)
    {
      Logger::write("No pipeline is set to bind", LogLevel::Error);
      return;
    }

    Pipeline &pipeline = *m_state.Pipeline;

#ifdef GAPI_GL
    GAPI_IF_GL(Application::graphics_api() == GraphicsAPI::OpenGL) {
      auto *data = reinterpret_cast<const Pipeline::GLState *>(m_state.Pipeline->get_id());
      glUseProgram(data->shader_program);
      glBindVertexArray(data->vao);
      // TODO: check for errors?
    }
#endif // GAPI_GL
  }

  void Renderer::_process_cmds() {
    for (size_t i = 0; i < m_commands->size(); i++)
    {
      _do_command((*m_commands)[i]);
    }
  }

  void Renderer::_do_command(const CommandInstance &command) {
    using rcq::CmdType;
    switch (command.get<rcq::Cmd>().get_type())
    {
    case CmdType::BindVertexBuffer:
      {
        const auto &cmd = command.get<rcq::CmdBindVertexBuffer>();
        const auto &vertex_buf = cmd.vertex_buffer;
        RT_ASSERT_RELEASE(vertex_buf->get_type() == StorageBufType::Vertex);
        set_buffer(*vertex_buf);
        return;
      }
    case CmdType::Draw:
      {
        const auto &cmd = command.get<rcq::CmdDraw>();
#ifdef GAPI_GL
        GAPI_IF_GL(GraphicsProfile::graphics_api() == GraphicsAPI::OpenGL) {
          const auto &input = m_state.Pipeline->get_create_info().input_states;
          gl_update_vao(input);
          glDrawArrays(gl_rt::to_gl_primitive(m_state.topology), cmd.offset, cmd.count);
          return;
        }
#endif // GAPI_GL


        return;
      }
    default:
      break;
    }
  }

}
