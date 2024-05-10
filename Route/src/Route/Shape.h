#pragma once
#include "Traits.h"

namespace route
{
  namespace shapes
  {

    enum class Type
    {
      Line2D,
      Triangle,
      Rectangle,
      Circle,

      Line3D,
      Plane,
      Pyramid,
      Box,
      Sphere
    };

    template <class _Triats>
    struct ShapeBase
    {
      using traits = _Triats;

      inline ShapeBase(const Type type) : m_type{ type } {

      }

      inline Type get_type() const noexcept {
        return m_type;
      }

    private:
      Type m_type;
    };

    struct Line2D : public ShapeBase<traits::Impl2D>
    {
      traits::vector start;
      traits::vector end;
    };

    struct Line3D : public ShapeBase<traits::Impl3D>
    {
      traits::vector start;
      traits::vector end;
    };

    struct Plane : public ShapeBase<traits::Impl3D>
    {
      traits::vector position;
      traits::vector normal;
      traits::lower_vector size;
    };

    struct Triangle : public ShapeBase<traits::Impl2D>
    {
      traits::vector position;
      traits::vector normal;
      traits::lower_vector base_size;
    };

    struct Pyramid : public ShapeBase<traits::Impl3D>
    {
      traits::vector position;
      traits::vector normal;
      traits::lower_vector base_size;
    };

    struct Rectangle : public ShapeBase<traits::Impl2D>
    {
      traits::vector position;
      traits::vector size;
    };

    struct Box : public ShapeBase<traits::Impl3D>
    {
      traits::vector position;
      traits::vector size;
    };

    struct Circle : public ShapeBase<traits::Impl2D>
    {
      real_t radius;
    };

    struct Sphere : public ShapeBase<traits::Impl3D>
    {
      real_t radius;
    };

  }

  union ShapeData
  {
  public:

    template <typename... _Args>
    ShapeData(shapes::Type type, _Args &&...args);
    ShapeData(const ShapeData &data);
    ShapeData &operator=(const ShapeData &data);
    ~ShapeData();

    template <typename _Ty>
    inline _Ty &get() {
      return *reinterpret_cast<_Ty *>(this);
    }

    template <typename _Ty>
    inline const _Ty &get() const {
      return *reinterpret_cast<const _Ty *>(this);
    }

    inline shapes::Type get_type() const {
      return m_type;
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
    shapes::Type m_type;
    shapes::Line2D m_line2d;
    shapes::Line3D m_line3d;
    shapes::Plane m_plane;
    shapes::Triangle m_triangle;
    shapes::Pyramid m_pyramid;
    shapes::Rectangle m_rect;
    shapes::Box m_box;
    shapes::Circle m_circle;
    shapes::Sphere m_sphere;
  };

  template<typename ..._Args>
  inline ShapeData::ShapeData(shapes::Type type, _Args && ...args) : m_type{ type } {
#define _INSTANCE_CASE(name) case shapes::Type::name: _instance<shapes::name>(std::forward<_Args>(args)...); break;
    switch (type)
    {
      _INSTANCE_CASE(Line2D);
      _INSTANCE_CASE(Line3D);
      _INSTANCE_CASE(Plane);
      _INSTANCE_CASE(Triangle);
      _INSTANCE_CASE(Pyramid);
      _INSTANCE_CASE(Rectangle);
      _INSTANCE_CASE(Box);
      _INSTANCE_CASE(Circle);
      _INSTANCE_CASE(Sphere);
    default:
      throw std::runtime_error("Unknown type");
#undef _INSTANCE_CASE
    }
  }

}
