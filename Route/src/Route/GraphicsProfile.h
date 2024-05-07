#pragma once
#include "Bases.h"

namespace route
{


  enum class GraphicsAPI
  {
    OpenGL,
    Vulkan,
  };

  class GraphicsProfile
  {
  public:

    // default to opengl
    static inline GraphicsAPI graphics_api() {
      return s_graphics_api;
    }

    // are we on opengl
    static FORCE_INLINE bool opengl() {
      return s_graphics_api == GraphicsAPI::OpenGL;
    }

    // are we on vulkan
    static FORCE_INLINE bool vulkan() {
      return s_graphics_api == GraphicsAPI::Vulkan;
    }

  private:
    static GraphicsAPI s_graphics_api;
  };

}
