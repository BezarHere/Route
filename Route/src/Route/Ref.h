#pragma once
#include "Bases.h"
#include <memory>


namespace route
{

  template <typename _Ty, typename _Dx = std::default_delete<_Ty>>
  class Ref
  {
  public:
    using value_type = _Ty;
    using deleter_type = _Dx;
    using this_type = Ref<_Ty, _Dx>;
    static constexpr bool IsArray = std::is_array_v<_Ty>;
    static constexpr bool AutoDestroy = true;

  private:

    inline void _delink() {
      m_ptr = nullptr;
      m_refc = nullptr;
    }

    inline void _destroy() {
#if _CONTAINER_DEBUG_LEVEL > 0
      if (!m_ptr || !m_refc || *m_refc)
        throw std::runtime_error("Invalid reference destroy");
#endif // _CONTAINER_DEBUG_LEVEL > 0
      deleter_type()(m_ptr);
      delete m_refc;
      _delink();
    }

    inline void _decref() {
      if (!m_refc)
        return;
      --(*m_refc);


      if (!*m_refc)
        _destroy();
    }

    inline void _incref() {
      if (!m_refc)
        return;
      ++(*m_refc);
    }

  public:

    inline constexpr Ref() noexcept : m_ptr{}, m_refc{} {
    }

    inline constexpr explicit Ref(value_type *ptr) noexcept : m_ptr{ ptr }, m_refc{ new size_t{1ULL} } {
    }

    inline ~Ref() noexcept {
      _decref();
    }

    inline Ref(const Ref &copy) : m_ptr{ copy.m_ptr }, m_refc{ copy.m_refc } {
      _incref(); // this object added it's self as a new reference
    }

    inline Ref(Ref &&move) noexcept : m_ptr{ move.m_ptr }, m_refc{ move.m_refc } {
      move._delink(); // this object took it's space
    }

    inline Ref &operator=(const Ref &copy) {
      // can't assign to self (cycle reference)
      if (&copy == this)
        return *this;

      // this object no longer reference the old data
      _decref();

      // setup the object to point to the new data
      m_ptr = copy.m_ptr;
      m_refc = copy.m_refc;

      // this object is now referencing the new data
      _incref();
    }

    inline Ref &operator=(Ref &&move) {
      // your fault to assign to self (lvalue to rvalue is already fucked)

      // this object no longer reference the old data
      _decref();

      // setup the object to point to the new data
      m_ptr = move.m_ptr;
      m_refc = move.m_refc;

      // this object is now taking the old object's place
      move._delink();
    }

    inline void swap(this_type &other) {
      const value_type *ptr = m_ptr;
      const volatile refc_t *refc = m_refc;

      m_ptr = other.m_ptr;
      m_refc = other.m_refc;

      other.m_ptr = ptr;
      other.m_refc = refc;
    }

    inline void clear() {
      _decref();
      _delink();
    }

    inline value_type *get() const {
      return m_ptr;
    }

    inline bool unique() const {
      return m_refc && *m_refc == 1;
    }

    inline refc_t use_count() const {
      std::shared_ptr<int> x;
      if (m_refc)
        return *m_refc;
      return 0;
    }

    inline value_type *operator->() const {
      return m_ptr;
    }

    inline value_type &operator*() const {
      return *m_ptr;
    }

    inline operator bool() const {
      return m_ptr;
    }

    inline value_type &operator[](const ptrdiff_t index) const {
      return m_ptr[index];
    }

  private:
    value_type *m_ptr;
    volatile refc_t *m_refc;
  };

  template <typename _Ty, typename... _Args>
  inline Ref<_Ty> make_ref(_Args &&...args) {
    return Ref<_Ty>(new _Ty(std::forward(args)...));
  }

}

