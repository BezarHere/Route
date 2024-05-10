#pragma once
#include "Bases.h"
#include "Traits.h"
#include "RenderCommands.h"

namespace route
{
  class Object;

  typedef intptr_t CompTypeID;

  class Component
  {
  public:
    enum CompFlags
    {
      eCompFlag_None = 0x0000,
      eCompFlag_Updated = 0x0001,
      eCompFlag_Drawen = 0x0002,
      eCompFlag_Physics = 0x0004,
    };

    inline Component(CompTypeID type, CompFlags flags)
      : type_id{ type }, flags{ flags } {
    }

    virtual bool compatible_with(const Object &object) const = 0;

    // update from the fixed-update thread, used to process input/post-rendering state
    // linked to the fps
    // needs a specific flag set to be engine-called
    virtual void update(real_t deltatime) = 0;
    // update from the physics thread, used to process post-physics actions
    // linked to the tickrate
    // needs a specific flag set to be engine-called
    virtual void update_physics(real_t deltatime) = 0;
    // forwards rendering commands to be drawn
    // needs a specific flag set to be engine-called
    virtual void draw(RenderCollector collector) = 0;

    virtual ~Component() {
    };

    const CompTypeID type_id;
    const CompFlags flags;
  };

  template <typename _Traits>
  class TSpaceComponent : public Component
  {
  public:
    using transform_type = typename _Traits::transform;
    using direction_type = typename _Traits::direction;

    inline TSpaceComponent(CompTypeID type, CompFlags flags, const transform_type &transform)
      : Component(type, flags), m_transform{ transform } {
    }

    static bool has_same_space(const Object &obj);

    inline bool compatible_with(const Object &object) const override {
      return has_same_space(object);
    }

    inline void set_transform(const transform_type &transform) {
      m_transform = transform;
    }

    inline const transform_type &get_transform() {
      return m_transform;
    }

  protected:
    transform_type m_transform;
  };

  using Component2D = TSpaceComponent<traits::Impl2D>;
  using Component3D = TSpaceComponent<traits::Impl3D>;

  class ShapeComp : public Component
  {
  public:
    static constexpr CompTypeID type_id = 0x1f2f3f4f;
    static constexpr CompFlags flags = (CompFlags)(eCompFlag_Drawen | eCompFlag_Updated);

    inline ShapeComp(const ShapeData &data)
      : Component(type_id, flags), shape{ data } {
    }

    inline bool compatible_with(const Object &object) const override {
      return true;
    }
    inline  void update(real_t deltatime) override { }
    inline void update_physics(real_t deltatime) override { }
    void draw(RenderCollector collector) override;

    ShapeData shape;
  };

  template <typename _Ty>
  _INLINE_VAR constexpr bool is_component_v =
    std::is_same_v<Component, _Ty> || std::is_base_of_v<Component, _Ty>;
  using component = Component;

}
