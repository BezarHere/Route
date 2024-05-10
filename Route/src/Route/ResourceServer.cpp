#include "pch.h"
#include "../pch.h"
#include "ResourceServer.h"
#include "Application.h"
#include "Logger.h"
#include "Image.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "StorageBuffer.h"
#include "Performance.h"
#include "HeapPool.h"

#include <list>
#include <sstream>

/*
* # RID should be a uint64_t composing of:
* 0xffffffff ffffffff (64bits)
*   ^^^^^^^^ [ index value (32) ]
*   |
*   [ check code (32) ]
*
*  ## the check code
*			varies between resource servers
*			it can be used to check if the rid is from any given resource server
*
*  ## the index value
*			contains where is the data allocated
*			it can have an extra security feature, but it's not required
*
*  ## NOTE: when encoding chunk/element index in the index value
*			the index value should have the chunk index on the upper bits
*			and the element/item index on the lower bits
*/


/*
* BOILER PLATE
*/

namespace route
{

  struct Application::TRSH
  {
  public:

    template <typename... _Tys>
    static inline void execute(bool state) {
      return state ? open<_Tys...>() : close<_Tys...>();
    }

    static inline void execute(bool state);

    template <typename _Ty>
    static inline void open() {
      Error err = ResourceServer<_Ty>::_open();
      if (err != Error::Ok)
      {
        std::ostringstream oss{};
        oss
          << "Couldn't start the resource server for the type \""
          << typeid(_Ty).name()
          << "\", error="
          << err;
        Logger::write(oss.str());
      }
    }

    template <typename _Ty>
    static inline void close() {
      ResourceServer<_Ty>::_close();
    }

    // two single parameters for overloading (packed args are optional which is not overloaded)
    template <typename _Ty, typename _Ey, typename... _Tys>
    static inline void open() {
      ResourceServer<_Ty>::_open();
      ResourceServer<_Ey>::_open();
      if constexpr (sizeof...(_Tys))
        TRSH::open<_Tys...>();
    }

    // two single parameters for overloading (packed args are optional which is not overloaded)
    template <typename _Ty, typename _Ey, typename... _Tys>
    static inline void close() {
      ResourceServer<_Ty>::_close();
      ResourceServer<_Ey>::_close();
      if constexpr (sizeof...(_Tys))
        TRSH::close<_Tys...>();
    }
  };

  template ResourceServer<Resource>;
  template ResourceServer<GraphicsResource>;
  template ResourceServer<IOResource>;
  template ResourceServer<Texture>;
  template ResourceServer<Material>;
  template ResourceServer<Shader>;
  template ResourceServer<StorageBuffer>;
  template ResourceServer<Pipeline>;

  inline void Application::TRSH::execute(bool state) {
    const auto mem = Performance::get_memory_usage();

    /*
    * Template defined below are which resource servers are to be template instanced
    * Not the best place to put such thing, but it works well
    */
    execute<Resource, GraphicsResource, IOResource, Texture, Material, Shader, StorageBuffer, Pipeline>(state);

    std::cout << "mem usage diff after " << (state ? "running" : "closing") << " the ResServers: "
      << (Performance::get_memory_usage() - mem) << '\n';

    return;
  }

  void Application::_toggle_resource_servers(uint32_t flags) {
    // TODO: add named flags
    const bool tobe_opened = flags & 1;
    return Application::TRSH::execute(tobe_opened);
  }

  struct RIndex
  {
    uint16_t chunk;
    uint16_t element;
  };

  template<typename _Ty>
  typename ResourceServer<_Ty>::Internal *ResourceServer<_Ty>::s_internal = nullptr;

  enum ElementFlags
  {
    EFlag_None = 0,
    EFlag_Init = 1,
  };
}

/*
* ACTUAL STUFF
*/

namespace route
{

  template<typename _Ty>
  struct ResourceServer<_Ty>::Internal {
    static constexpr size_t DefaultPoolSize = 1024;

    inline Internal() : pool(DefaultPoolSize) {

    }

    HeapPool<_Ty> pool;
    refc_t ref_counter;
  };

  template<typename _Ty>
  Error ResourceServer<_Ty>::_open() {
    if (s_internal)
    {
      s_internal->ref_counter++;
      return Error::Ok;
    }

    s_internal = new Internal();
    s_internal->ref_counter = 1;

#ifdef VERBOSE
    std::cout << "VERBOSE: " << typeid(ResourceServer<_Ty>).name() << " opened\n";
#endif

    return Error::Ok;
  }

  template<typename _Ty>
  bool ResourceServer<_Ty>::_close() {
    // no internals
    if (!s_internal)
    {
      char buf[256]{};
      (void)sprintf_s(buf, "%s already closed!", typeid(ResourceServer<_Ty>).name());
      std::_Xruntime_error(buf);
      //return false;
    }

    // was the ref_counter zero to overflow? then why isn't it deleted??
    if (--s_internal->ref_counter == npos)
    {
      char buf[256]{};
      (void)sprintf_s(
        buf,
        "%s has a corrupted internals ref_counter: %llu",
        typeid(ResourceServer<_Ty>).name(),
        s_internal->ref_counter
      );
      std::_Xruntime_error(buf);
      //return false;
    }

    // no more references to this resource server
    if (!s_internal->ref_counter)
    {
#ifdef VERBOSE
      size_t internals_size = sizeof(Internal);
      internals_size += s_internal->pool.size() * sizeof(_Ty);
#endif

      delete s_internal;
      s_internal = nullptr;

#ifdef VERBOSE
      std::cout << "VERBOSE: " << typeid(ResourceServer<_Ty>).name() << " closing (" << (internals_size >> 10) << " kb)\n";
#endif

      // the internals are deleted
      return true;
    }

    // the internals are still there
    return false;
  }
  template<typename _Ty>
  _Ty *ResourceServer<_Ty>::_allocate(size_t size) {
    if (!s_internal)
      return nullptr;
    return s_internal->pool.alloc(size);
  }

  template<typename _Ty>
  void ResourceServer<_Ty>::_destroy(_Ty *ptr) {
    if (!s_internal)
      return;
    return s_internal->pool.destroy(ptr);
  }
}
