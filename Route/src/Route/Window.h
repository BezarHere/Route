#pragma once
#include "Rect.h"

namespace route
{
  typedef void *WindowHandle;

  enum WindowFlags : unsigned
  {
    WindowFlag_Fullscreen = 0x00001,
    WindowFlag_Hidden = 0x00008,
    WindowFlag_Borderless = 0x0010,
    WindowFlag_Resizable = 0x00020,
    WindowFlag_Minimized = 0x00040,
    WindowFlag_Maximized = 0x00080,
    WindowFlag_AlwaysOnTop = 0x8000,
  };

  class Renderer;

  class Window
  {
    friend Renderer;
  public:

    Window(const std::string &title,
           const Rect2i &frame,
           WindowFlags flags = WindowFlag_Resizable);
    ~Window();

    Window(Window &);
    Window &operator=(Window &);
    Window(const Window &) = delete;
    Window &operator=(const Window &) = delete;

    Vec2u size() const;

    void poll();
    inline operator bool() const {
      return !m_should_close;
    }

    inline WindowHandle get_handle() const {
      return m_handle;
    }

  private:

    void _initialize();

  private:
    WindowHandle m_handle;
    bool m_should_close;
  };

}
