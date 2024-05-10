#pragma once
#include "Shader.h"
#include "VertexInputInfo.h"

namespace route
{

  typedef vpid_t PipelineID;

  struct PipelineCreateInfo
  {
    Blob<const Shader *> shaders;
    VertexInputInfo input_states;
  };


  // implementation GL: GL_SHADER
  // implementation VK: vkShaderModule
  class Pipeline : public GraphicsResource
  {
    friend GraphicsDevice;
  public:
    static constexpr size_t MaxShadersLinked = 6;

    struct GLState;
    struct VKState;
    Pipeline(Pipeline &&);
    Pipeline &operator=(Pipeline &&);
    ~Pipeline() noexcept;

    inline PipelineID get_id() const {
      return m_id;
    }

    inline const PipelineCreateInfo &get_create_info() const {
      return m_info;
    }

  private:
    Pipeline(const PipelineCreateInfo &info, device &device);
    Pipeline(const Pipeline &) = delete;
    Pipeline &operator=(const Pipeline &) = delete;

  private:
    PipelineID m_id;
    PipelineCreateInfo m_info;
  };


}

