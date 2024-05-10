#include "pch.h"
#include "GraphicsDevice.h"
#include "../pch.h"
#include "GraphicsProfile.h"
#include "Logger.h"
#include "Renderer.h"
#include "../gapi.h"
#include "internal/gl_utils.h"


/// @brief OPENGL: just returns a normal GL-context
/// @param param an SDL_Window on OpenGL and a <something in vulkan> on vulkan
/// @returns the device ID
static inline GraphicsDeviceID get_best_device(void *param) {
#ifdef GAPI_GL
  GAPI_IF_GL(GraphicsProfile::opengl()) {
    return OpenGL::create_context(reinterpret_cast<SDL_Window *>(param));
  }
#endif // GAPI_GL

#ifdef GAPI_VK
  GAPI_IF_VK(GraphicsProfile::vulkan()) {
    // TODO: score physical devices and create an optimal device
  }
#endif // GAPI_VK

}

static inline void destroy_device(GraphicsDeviceID id, void *param) {
  (void)param;

#ifdef GAPI_GL
  GAPI_IF_GL(GraphicsProfile::opengl()) {
    OpenGL::delete_context(reinterpret_cast<SDL_Window *>(id));
    return;
  }
#endif // GAPI_GL

#ifdef GAPI_VK
  GAPI_IF_VK(GraphicsProfile::vulkan()) {
    // TODO: destroy the device and do cleanup
  }
#endif // GAPI_VK
}

namespace route
{
  GraphicsDevice::GraphicsDevice(Window &window)
    : m_id{ get_best_device(window.get_handle()) },
    m_window{ window }, m_renderer{ *this },
    m_destroy_queues{} {

    if (!m_id)
    {
      const char msg[] = "Failed to create context for Renderer";
      Logger::write(msg, LogLevel::Error);
      throw std::runtime_error(msg);
    }

  }

  GraphicsDevice::~GraphicsDevice() {
    m_renderer._finalize();
    destroy_device(m_id, nullptr);
  }

  resource<StorageBuffer> GraphicsDevice::create_buffer(const StorageBufType type, size_t size) {
    if (!is_active())
      return {};
#ifdef GAPI_GL
    GAPI_IF_GL(GraphicsProfile::opengl()) {
      GLuint id = 0;
      glGenBuffers(1, &id);

      GLuint old_bind = OpenGL::query_int(gl_rt::get_buffer_type_binding(type));

      glBindBuffer(gl_rt::get_buffer_type(type), id);
      glBufferData(gl_rt::get_buffer_type(type), size, nullptr, GL_STATIC_DRAW);

      glBindBuffer(gl_rt::get_buffer_type(type), old_bind);
      return _create_resource<StorageBuffer, StorageBuffer>((StorageBufferID)id, type, size, *this);
    }
#endif
  }

  resource<Texture> GraphicsDevice::create_texture(const TextureInfo &info) {
    // TODO: stuff
    return {};
  }

  resource<Shader> GraphicsDevice::create_shader(const char *source, ShaderType type) {
    if (!is_active())
      return {};
#ifdef GAPI_GL
    return _create_resource<Shader, Shader>(source, type, *this);
#endif
    return {};
  }

  resource<Pipeline> GraphicsDevice::create_pipeline(const PipelineCreateInfo &info) {
    if (!is_active())
      return {};

    return _create_resource<Pipeline, Pipeline>(info, *this);
  }


  Error GraphicsDevice::update_buffer(StorageBuffer &buffer, const void *data, size_t length, size_t offset) const {
    if (!is_active())
      return Error::DeviceInactive;

#ifdef GAPI_GL
    GAPI_IF_GL(Application::graphics_api() == GraphicsAPI::OpenGL) {
      glBindBuffer(gl_rt::get_buffer_type(buffer.m_type), (GLuint)buffer.m_id);
      GL_CALL_POST(glBufferSubData(gl_rt::get_buffer_type(buffer.m_type), offset, length, data), return glErrRT(err));
      glBindBuffer(gl_rt::get_buffer_type(buffer.m_type), 0);
      return Error::Ok;
    }
#endif // GAPI_GL
  }

  void GraphicsDevice::_queue_free_buffer(const StorageBuffer &buffer) {
    if (!is_active())
    {
      m_destroy_queues.buffers.push_back({ buffer.get_id() });
      return;
    }
    _free_buffer({ buffer.get_id() });
  }

  void GraphicsDevice::_queue_free_texture(const Texture &texture) {
    if (!is_active())
    {
      m_destroy_queues.textures.push_back({ texture.get_id() });
      return;
    }
    _free_texture({ texture.get_id() });
  }

  void GraphicsDevice::_queue_free_shader(const Shader &shader) {
    if (!is_active())
    {
      m_destroy_queues.shaders.push_back({ shader.get_id() });
      return;
    }
    _free_shader({ shader.get_id() });
  }

  void GraphicsDevice::_queue_free_pipeline(const Pipeline &pipeline) {
    if (!is_active())
    {
      m_destroy_queues.pipelines.push_back({ pipeline.get_id() });
      return;
    }
    _free_pipeline({ pipeline.get_id() });
  }

  void GraphicsDevice::_free_buffer(const StorageBufQueueEntry &entry) {
#ifdef GAPI_GL
    GAPI_IF_GL(GraphicsProfile::graphics_api()) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteBuffers(1, &id);
    }
#endif // GAPI_GL
  }

  void GraphicsDevice::_free_texture(const TextureQueueEntry &entry) {
#ifdef GAPI_GL
    GAPI_IF_GL(GraphicsProfile::graphics_api()) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteTextures(1, &id);
    }
#endif // GAPI_GL
  }

  void GraphicsDevice::_free_shader(const ShaderQueueEntry &entry) {
#ifdef GAPI_GL
    GAPI_IF_GL(GraphicsProfile::graphics_api()) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteShader(id);
    }
#endif // GAPI_GL
  }

  void GraphicsDevice::_free_pipeline(const PipelineQueueEntry &entry) {
#ifdef GAPI_GL
    GAPI_IF_GL(GraphicsProfile::graphics_api()) {
      GLuint id = reinterpret_cast<GLuint>(entry);
      glDeleteProgram(id);
    }
#endif // GAPI_GL
  }

  void GraphicsDevice::_process_destroy_queues() {
    while (!m_destroy_queues.buffers.empty())
    {
      _free_buffer(m_destroy_queues.buffers.back());
      m_destroy_queues.buffers.pop_back();
    }

    while (!m_destroy_queues.textures.empty())
    {
      _free_texture(m_destroy_queues.textures.back());
      m_destroy_queues.textures.pop_back();
    }

    while (!m_destroy_queues.shaders.empty())
    {
      _free_shader(m_destroy_queues.shaders.back());
      m_destroy_queues.shaders.pop_back();
    }

    while (!m_destroy_queues.pipelines.empty())
    {
      _free_pipeline(m_destroy_queues.pipelines.back());
      m_destroy_queues.pipelines.pop_back();
    }
  }

  void GraphicsDevice::_begin() {
    SDL_FNCHECK_V(OpenGL::set_context(m_window.get_handle(), m_id), return);
    m_flags = eFlag_Active;

    _process_destroy_queues();
  }

  void GraphicsDevice::_end() {


    m_flags = eFlag_None;
  }

}
