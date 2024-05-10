#pragma once
#include "Bases.h"
#include "Resource.h"
#include "Ref.h"

namespace route
{
  class Application;
  class GraphicsDevice;

  /* MIGHT REMOVE THE RESOURCE SERVER IN THE FUTURE:
  *   the resource server's design is to encapsulate resource to manage their memory
  *   but with the addition of the Refs, having a central class manager seems more obsolete
  */

  template <typename _Ty>
  class ResourceServer
  {
    friend Application;
    friend GraphicsDevice;

    template <typename _Ty, typename _Base>
    friend struct ResourceDeleter;
  public:
    static_assert(std::is_same_v<Resource, _Ty> || std::is_base_of_v<Resource, _Ty>, "_TY should derive from Resource");

    struct Internal;

    using resource_type = _Ty;
    using resource_name_char = char;
    static constexpr size_t ResourceNameMaxLn = 128;

    /// @returns if the resource server is open and can be used
    static inline bool usable() {
      return s_internal != nullptr;
    }

    template <typename _Ey, typename... _Args>
    static resource<_Ty> create_resource(_Args &&...vargs);

    template <typename _Ey>
    static _Ey *_allocate();
    static void _destroy(_Ty *ptr);

  private:
    /// @note increments the ref-counter if the server is already running
    static Error _open();
    /// @returns true if the server closed, false otherwise (decremented ref counter)
    static bool _close();
    static _Ty *_allocate(size_t size);
  private:
    static Internal *s_internal;
  };

  template<typename _Ty>
  template<typename _Ey, typename ..._Args>
  inline resource<_Ty> ResourceServer<_Ty>::create_resource(_Args && ...args) {
    _Ey *ptr = _allocate(sizeof(_Ey));
    new(ptr) _Ey(args...);
    return resource(ptr);
  }

  template<typename _Ty>
  template<typename _Ey>
  inline _Ey *ResourceServer<_Ty>::_allocate() {
    return _allocate(sizeof(std::_Remove_cvref_t<_Ey>));
  }

  template<typename _Ty, typename _Base>
  inline void ResourceDeleter<_Ty, _Base>::operator()(_Ty *ptr) const {
    using server = ResourceServer<_Base>;
    server::_destroy(ptr);
  }

}
