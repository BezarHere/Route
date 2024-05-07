#pragma once

namespace route
{
  enum class PrimitiveTopology
  {
    Triangles,
    TriangleStrips,
    TriangleFans,
    Line,
    LineStrips,
    Points,
    Patches,
  };

  enum class RendererAttribute
  {
    DepthTest,
    // TODO: a lot of stuff (blending, scissors, ...)
  };

  enum class IndexType
  {
    Invalid = -1,
    UInt8,
    UInt16,
    UInt32,
  };

  enum class CompareOperator
  {
    Never,
    Less,
    Equal,
    LessOrEqual,
    Greater,
    NotEqual,
    GreaterOrEqual,
    Always
  };

  enum class CullMode
  {
    None,
    Front,
    Back,
    FrontAndBack
  };

  enum class FaceWinding
  {
    CounterClockwise,
    Clockwise,
  };
}
