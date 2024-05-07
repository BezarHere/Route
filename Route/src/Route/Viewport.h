#pragma once
#include "Rect.h"
#include "Range.h"

namespace route
{
  struct Viewport
  {
    Rect2f screen;
    FloatRange depth_range;
  };
}
