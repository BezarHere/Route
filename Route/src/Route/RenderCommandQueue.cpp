#include "pch.h"
#include "RenderCommandQueue.h"

namespace route
{

  CommandInstance::CommandInstance(const CommandInstance &copy) {

  }

  CommandInstance::CommandInstance(CommandInstance &&move) noexcept {
  }

  CommandInstance &CommandInstance::operator=(const CommandInstance &copy) {
    return *this;
  }

  CommandInstance &CommandInstance::operator=(CommandInstance &&move) noexcept {
    return *this;
  }

}
