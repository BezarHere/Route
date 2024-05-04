#include "pch.h"
#include "../pch.h"
#include "ShaderProgram.h"
#include "Logger.h"

static inline ShaderProgramID compile_program( const Blob<ShaderID> &shaders ) {
  GLuint prog = glCreateProgram();

  if (prog == 0)
  {
    Logger::write( format_join( "Failed to create shader program: ", glErrRT( glGetLastError() ) ) );
    return ShaderProgramID();
  }

  for (size_t i = 0; i < shaders.length; i++)
  {
    GL_CALL_POST(
      glAttachShader( prog, reinterpret_cast<GLuint>(shaders.data[ i ]) ),
      { Logger::write( format_join( "Attaching failed for shader No.", i, ", ID=", shaders.data[ i ] ) ); }
    );
  }

  GL_CALL( glLinkProgram( prog ) );

  GLint program_linked;
  glGetProgramiv( prog, GL_LINK_STATUS, &program_linked );

  if (!program_linked)
  {
    GLsizei len = 0;
    GLchar msg[ 1024 ];
    glGetProgramInfoLog( prog, 1024, &len, msg );
    Logger::write( format_join( "Failed to link shader program: ", msg ), LogLevel::Error );
    glDeleteProgram( prog );
    return 0;
  }

  return (ShaderID)prog;
}

static inline ShaderProgramID compile_program( const Blob<const Shader *> &shaders ) {
  ShaderID ids[ ShaderProgram::MaxShadersLinked ] = {};
  const size_t len = std::min( shaders.length, std::size( ids ) );

  for (size_t i = 0; i < len; i++)
  {
    // TODO: check for null shaders or invalid shader ids
    ids[ i ] = shaders.data[ i ]->get_id();
  }

  return compile_program( Blob<ShaderID>( ids, len ) );
}

static inline void destroy_program( ShaderProgramID prog ) {
#ifdef GAPI_GL
  GL_CALL( glDeleteProgram( (GLuint)prog ) );
#endif
}

namespace route
{
  ShaderProgram::ShaderProgram() : m_id{} {
  }

  ShaderProgram::ShaderProgram( const Shader &vertex, const Shader &fragment ) : ShaderProgram( { &vertex, &fragment } ) {
  }

  ShaderProgram::ShaderProgram( const Blob<const Shader *> &pShaders )
    : m_id{ compile_program( pShaders ) } {

  }

  ShaderProgram::ShaderProgram( ShaderProgram &&move ) : m_id{ move.m_id } {
    move.m_id = 0;
  }

  ShaderProgram &ShaderProgram::operator=( ShaderProgram &&move ) {
    destroy_program( m_id );
    m_id = move.m_id;
    move.m_id = 0;
    return *this;
  }

  ShaderProgram::~ShaderProgram() noexcept {
    destroy_program( m_id );
  }
}