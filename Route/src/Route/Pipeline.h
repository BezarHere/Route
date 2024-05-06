#pragma once
#include "Shader.h"
#include "VertexInputState.h"

namespace route
{

  typedef vpid_t PipelineID;

  class Pipeline
  {
  public:
    static constexpr size_t MaxShadersLinked = 6;

    Pipeline();
    Pipeline(const Blob<Shader *> &pShaders);
    Pipeline(Pipeline &&);
    Pipeline &operator=(Pipeline &&);
    ~Pipeline() noexcept;

    inline PipelineID get_id() const {
      return m_id;
    }

  private:
    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

  private:
    PipelineID m_id;
    VertexInputState m_input_state;
  };


}

