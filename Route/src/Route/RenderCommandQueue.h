#pragma once
#include "ArrayView.h"
#include "Enums.h"
#include "Primitives.h"
#include "ResourceServer.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "Vertex.h"
#include "StorageBuffer.h"
#include "Transform.h"
#include "Viewport.h"

namespace route
{
#pragma region(commands)

  namespace rcq
  {
    enum class CmdType
    {
      Draw,
      DrawIndexed,
      DrawPrimitive2D,
      DrawPrimitive3D,

      BindVertices,
      BindIndices,

      SetViewport,
      SetPrimitiveTopology,

      SetTransform2D,
      SetTransform3D,
      SetTexture,
      SetShader,
    };

    enum class Filter
    {
      Nearest,
      Linear,
    };

    enum class EdgeSampling
    {
      Repeate,
      MirrorRepeate,
      ClampToEdge,
      ClampToBoarder
    };

    struct Cmd
    {
    public:
      inline constexpr Cmd(CmdType type)
        : m_type{ type } {

      }

      FORCE_INLINE CmdType get_type() const {
        return m_type;
      }

    private:
      CmdType m_type;
    };

    template <typename _Ty, CmdType _Type>
    struct TCmdSWrapper : public Cmd
    {
      using wrapped_type = _Ty;
      inline TCmdSWrapper()

        : Cmd{ _Type }, value{} {
      }
      inline TCmdSWrapper(const wrapped_type &p_value)
        : Cmd{ _Type }, value{ p_value } {
      }

      wrapped_type value;
    };

    template <typename _Ty, CmdType _Type>
    struct TCmdSSetResource : public Cmd
    {
      using resource_type = _Ty;

      inline TCmdSSetResource()
        : Cmd{ _Type }, resource{} {
      }

      inline TCmdSSetResource(resource_ref<_Ty> p_resource)
        : Cmd{ _Type }, resource{ p_resource } {
      }

      inline bool can_invalidate() const {
        return ResourceServer<resource_type>::get_resource(resource).change_counter() != change_counter;
      }

      resource_ref<_Ty> resource;
      // for cache invalidation
      // if the resource has the same change counter, then it hasn't change
      refc_t change_counter;
    };

    struct CmdSetViewport : public Cmd
    {
      inline CmdSetViewport(Viewport p_viewport)
        : Cmd{ CmdType::SetViewport }, viewport{ p_viewport } {
      }

      Viewport viewport;
    };

    struct CmdSetPrimitiveTopology : public Cmd
    {
      inline CmdSetPrimitiveTopology(PrimitiveTopology p_topology = PrimitiveTopology::TriangleStrips)
        : Cmd{ CmdType::SetPrimitiveTopology }, topology{ p_topology } {
      }

      PrimitiveTopology topology;
    };


    // vertex buffer state is cleared after drawing
    struct CmdDraw : public Cmd
    {

      inline CmdDraw() : Cmd{ CmdType::Draw } {
      }

      inline CmdDraw(uint32_t p_count, uint32_t p_offset = 0)
        : Cmd{ CmdType::Draw }, count{ p_count }, offset{ p_offset } {
      }


      uint32_t count;
      uint32_t offset;
    };


    // vertex buffer state is cleared after drawing
    struct CmdDrawIndexed : public Cmd
    {

      inline CmdDrawIndexed() : Cmd{ CmdType::DrawIndexed } {
      }

      inline CmdDrawIndexed(uint32_t p_count, uint32_t p_offset = 0)
        : Cmd{ CmdType::DrawIndexed }, count{ p_count }, offset{ p_offset } {
      }


      uint32_t count;
      uint32_t offset;
    };

    struct CmdBindVertices : public Cmd
    {
      inline CmdBindVertices() : Cmd{ CmdType::BindVertices } {
      }

      inline CmdBindVertices(resource_ref<StorageBuffer> p_vertex_buffer)
        : Cmd{ CmdType::BindVertices },
        vertex_buffer{ p_vertex_buffer } {
      }

      resource_ref<StorageBuffer> vertex_buffer;
    };

    struct CmdBindIndices : public Cmd
    {
      inline CmdBindIndices() : Cmd{ CmdType::BindIndices } {
      }

      inline CmdBindIndices(resource_ref<StorageBuffer> p_vertex_buffer)
        : Cmd{ CmdType::BindIndices },
        vertex_buffer{ p_vertex_buffer } {
      }

      resource_ref<StorageBuffer> vertex_buffer;
    };


    using CmdSetTransform2D = TCmdSWrapper<Transform2D, CmdType::SetTransform2D>;
    using CmdSetTransform3D = TCmdSWrapper<Transform3D, CmdType::SetTransform3D>;

    using CmdDrawPrimitive2D = TCmdSWrapper<boxed_primitive_2d, CmdType::DrawPrimitive2D>;
    using CmdDrawPrimitive3D = TCmdSWrapper<boxed_primitive_3d, CmdType::DrawPrimitive3D>;

    using CmdSetTexture = TCmdSWrapper<Texture, CmdType::SetTexture>;
    using CmdSetShader = TCmdSWrapper<Shader, CmdType::SetShader>;

    using CmdSetViewport = CmdSetViewport;
    using CmdSetPrimitiveTopology = CmdSetPrimitiveTopology;

    using CmdDraw = CmdDraw;
    using CmdDrawIndexed = CmdDrawIndexed;

    using CmdBindVertices = CmdBindVertices;
    using CmdBindIndices = CmdBindIndices;

  }
#pragma endregion

  union CommandInstance
  {
  public:
    using CmdType = rcq::CmdType;

    template <typename... _Args>
    inline CommandInstance(rcq::CmdType type, _Args &&...args);
    inline ~CommandInstance();

    CommandInstance(const CommandInstance &copy);
    CommandInstance(CommandInstance &&move) noexcept;
    CommandInstance &operator=(const CommandInstance &copy);
    CommandInstance &operator=(CommandInstance &&move) noexcept;

    template <typename _Ty = rcq::Cmd>
    inline _Ty &get() {
      return *reinterpret_cast<_Ty *>(this);
    }

    template <typename _Ty = rcq::Cmd>
    inline const _Ty &get() const {
      return *reinterpret_cast<const _Ty *>(this);
    }

    inline rcq::CmdType get_type() const {
      return m_base.get_type();
    }

  private:

    template <typename _Ty, typename... _Args>
    FORCE_INLINE void _instance(_Args &&...args) {
      new(this) _Ty(std::forward<_Args>(args)...);
    }

    template <typename _Ty>
    FORCE_INLINE void _dtor() {
      ((_Ty *)this)->~_Ty();
    }

  private:
    rcq::Cmd m_base;
    rcq::CmdSetTransform2D m_xform2d;
    rcq::CmdSetTransform3D m_xform3d;
    rcq::CmdDrawPrimitive2D m_primitive2d;
    rcq::CmdDrawPrimitive3D m_primitive3d;
    rcq::CmdSetTexture m_texture;
    rcq::CmdSetShader m_shader;
    rcq::CmdDraw m_draw_vertices;
    rcq::CmdBindVertices m_bind_vertex_source;
  };

  template<typename ..._Args>
  inline CommandInstance::CommandInstance(CmdType type, _Args && ...args) : m_base{ type } {
#define _INSTANCE_CALL(type) _instance<type>(std::forward<_Args>(args)...)
    switch (type)
    {
    case CmdType::Draw:
      _INSTANCE_CALL(rcq::CmdDraw);
      break;
    case CmdType::DrawIndexed:
      _INSTANCE_CALL(rcq::CmdDrawIndexed);
      break;
    case CmdType::DrawPrimitive2D:
      _INSTANCE_CALL(rcq::CmdDrawPrimitive2D);
      break;
    case CmdType::DrawPrimitive3D:
      _INSTANCE_CALL(rcq::CmdDrawPrimitive3D);
      break;

    case CmdType::BindVertices:
      _INSTANCE_CALL(rcq::CmdBindVertices);
      break;
    case CmdType::BindIndices:
      _INSTANCE_CALL(rcq::CmdBindIndices);
      break;

    case CmdType::SetPrimitiveTopology:
      _INSTANCE_CALL(rcq::CmdSetPrimitiveTopology);
      break;
    case CmdType::SetViewport:
      _INSTANCE_CALL(rcq::CmdSetViewport);
      break;

    case CmdType::SetTransform2D:
      _INSTANCE_CALL(rcq::CmdSetTransform2D);
      break;
    case CmdType::SetTransform3D:
      _INSTANCE_CALL(rcq::CmdSetTransform3D);
      break;

    case CmdType::SetTexture:
      _INSTANCE_CALL(rcq::CmdSetTexture);
      break;
    case CmdType::SetShader:
      _INSTANCE_CALL(rcq::CmdSetShader);
      break;
    default:
      break;
    }
#undef _INSTANCE_CALL
  }

  inline CommandInstance::~CommandInstance() {
    switch (get_type())
    {
    case CmdType::Draw:
      _dtor<rcq::CmdDraw>();
      break;
    case CmdType::DrawIndexed:
      _dtor<rcq::CmdDrawIndexed>();
      break;
    case CmdType::DrawPrimitive2D:
      _dtor<rcq::CmdDrawPrimitive2D>();
      break;
    case CmdType::DrawPrimitive3D:
      _dtor<rcq::CmdDrawPrimitive3D>();
      break;

    case CmdType::BindVertices:
      _dtor<rcq::CmdBindVertices>();
      break;
    case CmdType::BindIndices:
      _dtor<rcq::CmdBindIndices>();
      break;

    case CmdType::SetPrimitiveTopology:
      _dtor<rcq::CmdSetPrimitiveTopology>();
      break;
    case CmdType::SetViewport:
      _dtor<rcq::CmdSetViewport>();
      break;

    case CmdType::SetTransform2D:
      _dtor<rcq::CmdSetTransform2D>();
      break;
    case CmdType::SetTransform3D:
      _dtor<rcq::CmdSetTransform3D>();
      break;

    case CmdType::SetTexture:
      _dtor<rcq::CmdSetTexture>();
      break;
    case CmdType::SetShader:
      _dtor<rcq::CmdSetShader>();
      break;
    default:
      _dtor<rcq::Cmd>();
      break;
    }
  }

}
