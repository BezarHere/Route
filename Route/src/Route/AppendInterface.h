#pragma once

namespace route
{

  template <typename _Ty>
  class AppendInterface
  {
  public:
    inline AppendInterface(_Ty &container) : m_container{ container } {
    }

    inline const _Ty &get_container() const {
      return m_container;
    }

    inline const _Ty *operator->() const {
      return &m_container;
    }

    template <typename... _Args>
    inline auto emplace_back(_Args &&... args) {
      m_container.emplace_back(args);
    }

  private:
    _Ty &m_container;
  };

}
