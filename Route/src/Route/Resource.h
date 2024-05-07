#pragma once
#include "Bases.h"
#include "Error.h"
#include "Ref.h"


namespace route
{
  template <typename _Ty, typename _Base = _Ty>
  struct ResourceDeleter
  {
    inline void operator()(_Ty *ptr) const;
  };

  template <typename _Ty, typename _Base = _Ty>
  using resource_ref = Ref<_Ty, ResourceDeleter<_Ty, _Base>>;

  class Resource
  {
  public:
    inline refc_t change_counter() const {
      return m_change_counter;
    }

  protected:
    inline void inc_change_counter() {
      ++m_change_counter;
    };
  private:
    refc_t m_change_counter;
  };

  class GraphicsDevice;
  class Renderer;
  // resource that are create or handled by a GraphicsDevice
  class GraphicsResource : public Resource
  {
    friend GraphicsDevice;
    friend Renderer;
  public:
    using device = GraphicsDevice;

    inline device &get_device() const {
      return m_device;
    }

  protected:
    inline GraphicsResource(device &device) : m_device{ device } {
    }

  private:
    device &m_device;
  };

  // base class for all resources that can be load/saved to a file in the disk
  class IOResource : public Resource
  {
  public:

    inline virtual Error save(const string &path) const {
      (void)path;
      return Error::NotImplemented;
    }

    inline virtual Error load(const string &path) {
      (void)path;
      return Error::NotImplemented;
    }

  protected:
    string m_filepath;
  };

  // base class for all resources that can be load/saved to a file in the disk
  class UserResource : public Resource
  {
  public:

  protected:
    void *m_data;
  };
}
