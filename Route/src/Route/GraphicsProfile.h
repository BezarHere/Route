#pragma once


namespace route
{


  enum class GraphicsAPI
  {
    OpenGL,
    //Vulkan,
  };

  class GraphicsProfile
  {
  public:

    // default to opengl
    static inline GraphicsAPI graphics_api() {
      return s_graphics_api;
    }

  private:
    static GraphicsAPI s_graphics_api;
  };

}
