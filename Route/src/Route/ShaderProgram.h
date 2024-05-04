#pragma once
#include "Shader.h"

namespace route
{

  typedef vpid_t ShaderProgramID;

  class ShaderProgram
  {
  public:
    static constexpr size_t MaxShadersLinked = 6;

    ShaderProgram();
    ShaderProgram( const Shader &vertex, const Shader &fragment );
    ShaderProgram( const Blob<const Shader *> &pShaders );
    ShaderProgram( ShaderProgram && );
    ShaderProgram &operator=( ShaderProgram && );
    ~ShaderProgram() noexcept;

    inline ShaderProgramID get_id() const {
      return m_id;
    }

  private:
    ShaderProgram( const ShaderProgram & ) = delete;
    ShaderProgram &operator=( const ShaderProgram & ) = delete;

  private:
    ShaderProgramID m_id;
  };


}

