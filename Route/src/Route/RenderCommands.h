#pragma once
#include "ArrayView.h"
#include "Enums.h"
#include "Shape.h"
#include "ResourceServer.h"
#include "StaticSpan.h"
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

      DrawShape,

      BindVertexBuffer,
      BindIndexBuffer,

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
      inline TCmdSSetResource(resource<_Ty> p_resource)
        : Cmd{ _Type }, resource{ p_resource } {
      }

      inline bool can_invalidate() const {
        return ResourceServer<resource_type>::get_resource(resource).change_counter() != change_counter;
      }

      resource<_Ty> resource;
      // for cache invalidation
      // if the resource has the same change counter, then it hasn't change
      refc_t change_counter;
    };

    struct CmdDrawShape : public Cmd
    {
      inline CmdDrawShape(const ShapeData &p_value)
        : Cmd{ CmdType::DrawShape }, value{ p_value } {
      }

      ShapeData value;
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

    struct CmdBindVertexBuffer : public Cmd
    {
      inline CmdBindVertexBuffer() : Cmd{ CmdType::BindVertexBuffer } {
      }

      inline CmdBindVertexBuffer(resource<StorageBuffer> p_vertex_buffer)
        : Cmd{ CmdType::BindVertexBuffer },
        vertex_buffer{ p_vertex_buffer } {
      }

      resource<StorageBuffer> vertex_buffer;
    };

    struct CmdBindIndexBuffer : public Cmd
    {
      inline CmdBindIndexBuffer() : Cmd{ CmdType::BindIndexBuffer } {
      }

      inline CmdBindIndexBuffer(resource<StorageBuffer> p_vertex_buffer)
        : Cmd{ CmdType::BindIndexBuffer },
        vertex_buffer{ p_vertex_buffer } {
      }

      resource<StorageBuffer> vertex_buffer;
    };


    using CmdSetTransform2D = TCmdSWrapper<Transform2D, CmdType::SetTransform2D>;
    using CmdSetTransform3D = TCmdSWrapper<Transform3D, CmdType::SetTransform3D>;

    using CmdDrawShape = CmdDrawShape;

    using CmdSetTexture = TCmdSSetResource<Texture, CmdType::SetTexture>;
    using CmdSetShader = TCmdSSetResource<Shader, CmdType::SetShader>;

    using CmdSetViewport = CmdSetViewport;
    using CmdSetPrimitiveTopology = CmdSetPrimitiveTopology;

    using CmdDraw = CmdDraw;
    using CmdDrawIndexed = CmdDrawIndexed;

    using CmdBindVertexBuffer = CmdBindVertexBuffer;
    using CmdBindIndexBuffer = CmdBindIndexBuffer;

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
      if constexpr (std::is_constructible_v<_Ty, _Args...>)
        new(this) _Ty(std::forward<_Args>(args)...);
      // TODO: what should happen if this is called when we are unable to create _Ty?
    }

    template <typename _Ty>
    FORCE_INLINE void _dtor() {
      ((_Ty *)this)->~_Ty();
    }

  private:
    rcq::Cmd m_base;
    rcq::CmdSetTransform2D m_xform2d;
    rcq::CmdSetTransform3D m_xform3d;
    rcq::CmdDrawShape m_shape;
    rcq::CmdSetTexture m_texture;
    rcq::CmdSetShader m_shader;
    rcq::CmdDraw m_draw_vertices;
    rcq::CmdBindVertexBuffer m_bind_vertex_source;
  };


  class RenderCollector
  {
    friend Renderer;
  public:
    // 2mb worth of command instances
    static constexpr size_t DefaultCmdBufSize = 0x200000ULL / sizeof(CommandInstance);
    using container_type = StaticSpan<CommandInstance, DefaultCmdBufSize>;

    FORCE_INLINE const container_type &get_container() const {
      return m_container;
    }

    FORCE_INLINE const container_type *operator->() const {
      return &m_container;
    }

    template <typename... _Args>
    FORCE_INLINE auto emplace_back(_Args &&... args) {
      m_container.emplace_back(args);
    }

  private:

    FORCE_INLINE RenderCollector(container_type &container) : m_container{ container } {
    }

  private:
    container_type &m_container;
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
    case CmdType::DrawShape:
      _INSTANCE_CALL(rcq::CmdDrawShape);
      break;

    case CmdType::BindVertexBuffer:
      _INSTANCE_CALL(rcq::CmdBindVertexBuffer);
      break;
    case CmdType::BindIndexBuffer:
      _INSTANCE_CALL(rcq::CmdBindIndexBuffer);
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
    case CmdType::DrawShape:
      _dtor<rcq::CmdDrawShape>();
      break;

    case CmdType::BindVertexBuffer:
      _dtor<rcq::CmdBindVertexBuffer>();
      break;
    case CmdType::BindIndexBuffer:
      _dtor<rcq::CmdBindIndexBuffer>();
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
