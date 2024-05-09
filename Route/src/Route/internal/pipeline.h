#pragma once
#include "../../gapi.h"
#include "../Pipeline.h"

namespace route
{
  struct Pipeline::GLState
  {
#ifdef GAPI_GL
    GLuint shader_program;
    GLuint vao;
#endif
  };


  struct Pipeline::VKState
  {
#ifdef GAPI_VK
    vkPipeline pipeline;
    VkGraphicsPipelineCreateInfo create_info;
#endif
  };
}

namespace _rt
{


}
