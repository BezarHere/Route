#pragma once
#include "Transform.h"

#pragma warning(push)
#pragma warning(disable:4624)

namespace route
{
  namespace traits
  {
    class BaseTrait
    {
    private:
      // can't be constructed/deleted
      BaseTrait() = delete;
      ~BaseTrait() = delete;
    };

    class Impl2D : BaseTrait
    {
    public:
      using transform = Transform2D;
      using direction = typename transform::vector_type;
      using vector = typename transform::vector_type;
      using lower_vector = typename vector::lower_type;
      using zindex = int16_t;
    };

    class Impl3D : BaseTrait
    {
    public:
      using transform = Transform3D;
      using direction = typename transform::vector_type;
      using vector = typename transform::vector_type;
      using lower_vector = typename vector::lower_type;
      using zindex = nothing;
    };
  }
}

#pragma warning(pop)
