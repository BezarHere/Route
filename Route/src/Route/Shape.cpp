#include "pch.h"
#include "Shape.h"
#include "Logger.h"

namespace route
{

  ShapeData::ShapeData(const ShapeData &data) : m_type{ data.m_type } {
#define _COPY_CASE(name) case shapes::Type::name: _instance<shapes::name>(data.get<shapes::name>()); return
    switch (m_type)
    {
      _COPY_CASE(Line2D);
      _COPY_CASE(Line3D);
      _COPY_CASE(Plane);
      _COPY_CASE(Triangle);
      _COPY_CASE(Pyramid);
      _COPY_CASE(Rectangle);
      _COPY_CASE(Box);
      _COPY_CASE(Circle);
      _COPY_CASE(Sphere);
    default:
      Logger::write(format_join("COPY: Unknown shape type ", (int)m_type), LogLevel::Warnning);
      return;
    }
#undef _COPY_CASE
  }

  ShapeData &ShapeData::operator=(const ShapeData &data) {
#define _ASSIGN_CASE(name) case shapes::Type::name: get<shapes::name>() = data.get<shapes::name>(); break;
    switch (m_type)
    {
      _ASSIGN_CASE(Line2D);
      _ASSIGN_CASE(Line3D);
      _ASSIGN_CASE(Plane);
      _ASSIGN_CASE(Triangle);
      _ASSIGN_CASE(Pyramid);
      _ASSIGN_CASE(Rectangle);
      _ASSIGN_CASE(Box);
      _ASSIGN_CASE(Circle);
      _ASSIGN_CASE(Sphere);
    default:
      Logger::write(format_join("ASSIGN: Unknown shape type ", (int)m_type), LogLevel::Warnning);
      break;
    }
    return *this;
#undef _ASSIGN_CASE
  }

  ShapeData::~ShapeData() {
#define _DTOR_CASE(name) case shapes::Type::name: _dtor<shapes::name>(); return
    switch (m_type)
    {
      _DTOR_CASE(Line2D);
      _DTOR_CASE(Line3D);
      _DTOR_CASE(Plane);
      _DTOR_CASE(Triangle);
      _DTOR_CASE(Pyramid);
      _DTOR_CASE(Rectangle);
      _DTOR_CASE(Box);
      _DTOR_CASE(Circle);
      _DTOR_CASE(Sphere);
    default:
      Logger::write(format_join("DTOR: Unknown shape type ", (int)m_type), LogLevel::Warnning);
      return;
    }
#undef _DTOR_CASE
  }

}