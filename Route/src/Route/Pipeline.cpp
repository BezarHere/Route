#include "pch.h"
#include "../pch.h"
#include "Pipeline.h"
#include "internal/pipeline.h"
#include "GraphicsDevice.h"
#include "Logger.h"
#include "ResourceServer.h"

static inline GLuint compile_program(const Blob<ShaderID> &shaders) {
  GLuint prog = glCreateProgram();

  if (prog == 0)
  {
    Logger::write(format_join("Failed to create shader program: ", glErrRT(glGetLastError())));
    return GLuint();
  }

  for (size_t i = 0; i < shaders.length; i++)
  {
    GL_CALL_POST(
      glAttachShader(prog, reinterpret_cast<GLuint>(shaders.data[i])),
      { Logger::write(format_join("Attaching failed for shader No.", i, ", ID=", shaders.data[i])); }
    );
  }

  GL_CALL(glLinkProgram(prog));

  GLint program_linked;
  glGetProgramiv(prog, GL_LINK_STATUS, &program_linked);

  if (!program_linked)
  {
    GLsizei len = 0;
    GLchar msg[1024];
    glGetProgramInfoLog(prog, 1024, &len, msg);
    Logger::write(format_join("Failed to link shader program: ", msg), LogLevel::Error);
    glDeleteProgram(prog);
    return 0;
  }

  return prog;
}

static inline GLuint compile_program(const Blob<const Shader *> &shaders) {
  ShaderID ids[Pipeline::MaxShadersLinked] = {};
  const size_t len = std::min(shaders.length, std::size(ids));

  for (size_t i = 0; i < len; i++)
  {
    // TODO: check for nulls
    ids[i] = shaders.data[i]->get_id();
  }

  return compile_program(Blob<ShaderID>(ids, len));
}

// TODO: move to the graphics device source
static inline PipelineID create_pipeline(const PipelineCreateInfo &info, GraphicsDevice &device) {
#ifdef GAPI_GL
  GAPI_IF_GL(GraphicsProfile::opengl()) {
    Pipeline::GLState *state = new Pipeline::GLState();
    state->shader_program = compile_program(info.shaders);
    glCreateVertexArrays(1, &state->vao);
    return (PipelineID)state;
  }
#endif
}

namespace route
{
  Pipeline::Pipeline(const PipelineCreateInfo &info, device &device)
    : GraphicsResource(device), m_id{ create_pipeline(info, device) }, m_info{ info } {

  }

  Pipeline::Pipeline(Pipeline &&move) : GraphicsResource(move.get_device()), m_id{ move.m_id } {
    move.m_id = 0;
  }

  Pipeline &Pipeline::operator=(Pipeline &&move) {
    if (!this->valid_assign(move))
      return *this;

    get_device()._queue_free_pipeline(*this);
    m_id = move.m_id;
    move.m_id = 0;
    return *this;
  }

  Pipeline::~Pipeline() noexcept {
    get_device()._queue_free_pipeline(*this);
  }
}