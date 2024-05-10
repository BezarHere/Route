#pragma once
#include "Boxed.h"
#include "Transform.h"

namespace route
{
  namespace shapes
  {
    enum class ShapeType2D
    {
      Rect,
      Circle,
      Triangle,
      Line,
    };

    enum class ShapeType3D
    {
      Cube,
      Sphere,
      Pyrimid,
      Plane,
      Tupe,
    };

    template <typename _Traits>
    struct TShapeDataND
    {
      template <typename>
      struct _type_enum;

      template <>
      struct _type_enum<traits::Impl2D> {
        using type = ShapeType2D;
      };

      template <>
      struct _type_enum<traits::Impl3D> {
        using type = ShapeType3D;
      };

      using traits = _Traits;
      using this_type = TShapeDataND<_Traits>;
      using transform_type = typename _Traits::transform;
      using vector_type = typename _Traits::direction;
      using real_type = typename vector_type::value_type;
      using type_enum = typename _type_enum<_Traits>::type; /* ridiculous */

      // more typing
      inline TShapeDataND(const type_enum type) : m_type{ type } {
      }

      inline type_enum get_type() const {
        return m_type;
      }

    private:
      type_enum m_type;
    };

    using ShapeData2D = TShapeDataND<traits::Impl2D>;
    using ShapeData3D = TShapeDataND<traits::Impl3D>;

    struct RectShape : public ShapeData2D
    {
      inline RectShape()
        : ShapeData2D(ShapeData2D::type_enum::Rect) {
      }

      inline RectShape(const Rect2f &p_rect)
        : ShapeData2D(ShapeData2D::type_enum::Rect), rect{ p_rect } {
      }

      Rect2f rect;
    };

    struct CircleShape : public ShapeData2D
    {
      inline CircleShape()
        : ShapeData2D(ShapeData2D::type_enum::Circle) {
      }

      inline CircleShape(const real_type p_radius)
        : ShapeData2D(ShapeData2D::type_enum::Circle), radius{ p_radius } {
      }

      real_type radius;
    };

  }

  union PrimitiveShape2D
  {
  public:
    using shape_data_type = shapes::ShapeData2D;
    using traits = typename shape_data_type::traits;
    using type_enum = typename shape_data_type::type_enum;

    template <typename... _Args>
    PrimitiveShape2D(type_enum type, _Args &&...args);
    PrimitiveShape2D(const PrimitiveShape2D &copy);
    PrimitiveShape2D &operator=(const PrimitiveShape2D &copy);
    ~PrimitiveShape2D();

    template <typename _Ty>
    inline _Ty &get() {
      static_assert(std::is_convertible_v<_Ty, shape_data_type>, "Unrecognized _Ty");
      return *reinterpret_cast<_Ty *>(this);
    }

    template <typename _Ty>
    inline const _Ty &get() const {
      static_assert(std::is_convertible_v<_Ty, shape_data_type>, "Unrecognized _Ty");
      return *reinterpret_cast<_Ty *>(this);
    }

  private:

    template <typename _Ty, typename... _Args>
    FORCE_INLINE void _instance(_Args &&...args) {
      if constexpr (std::is_constructible_v<_Ty, _Args...>)
        new(this) _Ty(std::forward<_Args>(args)...);
      // TODO: what should happen if this is called without creating _Ty?
    }

    template <typename _Ty>
    FORCE_INLINE void _dtor() {
      ((_Ty *)this)->~_Ty();
    }

  private:
    shape_data_type m_base;
    shapes::RectShape m_rect;
    shapes::CircleShape m_circle;
  };

  union PrimitiveShape3D
  {
  public:
    using shape_data_type = PrimitiveData3D;
    using traits = typename shape_data_type::traits;

  private:

  };

  template<typename ..._Args>
  inline PrimitiveShape2D::PrimitiveShape2D(type_enum type, _Args && ...args) : m_base{ type } {
    switch (type)
    {
    case type_enum::Circle:
      _instance<CirclePrimitive>(std::forward<_Args>(args)...);
      break;
    case type_enum::Rect:
      _instance<RectPrimitive>(std::forward<_Args>(args)...);
      break;
    default:
      break;
    }
  }

}
