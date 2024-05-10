#pragma once
#include "Bases.h"
#include <vector>

namespace route
{

  template <typename _Ty, size_t _Sz>
  class StaticSpan
  {
  public:
    using value_type = _Ty;
    static constexpr size_t max_size = _Sz;

    using size_type = size_t;
    using this_type = StaticSpan<_Ty, _Sz>;

    inline value_type *_Data() {
      return reinterpret_cast<value_type *>(m_memory);
    }

    inline const value_type *_Data() const {
      return reinterpret_cast<const value_type *>(m_memory);
    }

    inline value_type *_Begin() {
      return _Data();
    }

    inline const value_type *_Begin() const {
      return _Data();
    }

    inline value_type *_End() {
      return _Data() + m_size;
    }

    inline const value_type *_End() const {
      return _Data() + m_size;
    }

    template <typename... _Args>
    inline void _Construct(value_type *ptr, _Args &&...args) {
      new(ptr) value_type(std::forward<_Args>(args)...);
    }

    inline void _Destroy(value_type *const ptr) {
      (void)ptr;
      if constexpr (!std::is_trivially_destructible_v<value_type>)
      {
        ptr->~value_type();
      }
    }

    template <typename... _Args>
    inline value_type &_Emplace_back(_Args &&... args) {
      if (m_size == max_size)
        throw std::length_error("StaticSpan is full");
      value_type *const ptr = &_Data()[m_size++];
      _Construct(ptr, std::forward<_Args>(args)...);
      return *ptr;
    }

    inline void _Destroy_back() {
      _Destroy(&_Data()[m_size - 1]);
    }

    inline void _Destroy_all() {
      value_type *const _data = _Data();
      for (size_t i = 0; i < m_size; ++i)
        _Destroy(_data + i);
    }

    inline void _Shift_range(const std::pair<size_type, size_type> range, ptrdiff_t offset) {

      const size_t _length = range.second - range.first;
      value_type *const _data = _Data() + range.first;

      // shifting down, start from begin
      if (offset < 0)
      {
        for (size_t i = 0; i < _length; i++)
        {
          // data is occupied begin to end, so all of the elements under us are occupied
          _Destroy(&_data[offset + i]);
          _Construct(&_data[offset + i], std::move(_data[i]));
        }
      }
      // shifting up, start from end
      else
      {
        for (size_t i = 1; i <= _length; i++)
        {
          // shifting to occupied space, dtor element
          if (offset + _length - i < m_size)
            _Destroy(&_data[offset + _length - i]);

          _Construct(&_data[offset + _length - i], std::move(_data[_length - i]));
        }
      }
    }

    // does not check for ready elements nor destroys them, only does managed COPY
    inline void _Copy(const value_type *const begin, const value_type *const end) {
      const size_t _length = end - begin;
      value_type *const _data = _Data();

      for (size_t i = 0; i < _length; i++)
      {
        _Construct(_data + i, begin[i]);
      }
    }

    // does not check for ready elements nor destroys them, only does managed MOVE
    inline void _Move(const value_type *const begin, const value_type *const end) {
      const size_t _length = end - begin;
      value_type *const _data = _Data();

      for (size_t i = 0; i < _length; i++)
      {
        _Construct(_data + i, std::move(begin[i]));
      }
    }

    inline StaticSpan() : m_size{} {
    }

    inline StaticSpan(const std::initializer_list<value_type> &list) : m_size{ list.size() } {
      if (list.size() > max_size)
        throw std::length_error("StaticSpan: list too big");

      _Copy(list.begin(), list.end());
    }

    template <size_t _Sz2>
    inline StaticSpan(StaticSpan<_Ty, _Sz2> &&move) : m_size{ move.size() } {
      if (max_size < move.m_size)
        throw std::length_error("StaticSpan: can't move from a larger span");
      _Move(move._Begin(), move._End());
      move.m_size = 0;
    }

    template <size_t _Sz2>
    inline StaticSpan(const StaticSpan<_Ty, _Sz2> &copy) : m_size{ copy.size() } {
      if (max_size < copy.size())
        throw std::length_error("StaticSpan: can't copy from a larger span");
      _Copy(copy._Begin(), copy._End());
    }

    template <size_t _Sz2>
    inline StaticSpan &operator=(StaticSpan<_Ty, _Sz2> &&move) {
      if (max_size < move.m_size)
        throw std::length_error("StaticSpan: can't assign move from a larger span");

      _Destroy_all();

      _Move(move._Begin(), move._End());
      m_size = move.m_size;
      move.m_size = 0;
      return *this;
    }

    template <size_t _Sz2>
    inline StaticSpan &operator=(const StaticSpan<_Ty, _Sz2> &copy) {
      if (max_size < copy.m_size)
        throw std::length_error("StaticSpan: can't assign copy from a larger span");

      if (this == &copy)
        return *this;

      _Destroy_all();

      _Copy(copy._Begin(), copy._End());
      m_size = copy.m_size;
      return *this;
    }

    template <size_t _Sz2>
    inline bool operator==(const StaticSpan<_Ty, _Sz2> &other) const {
      if (other.m_size != m_size)
        return false;
      return std::equal(_Begin(), _End(), other._Begin());
    }

    template <size_t _Sz2>
    inline bool operator!=(const StaticSpan<_Ty, _Sz2> &other) const {
      return !(*this == other);
    }

    inline ~StaticSpan() {
      _Destroy_all();
    }

    inline size_t capacity() const {
      return max_size;
    }

    inline size_t size() const {
      return m_size;
    }

    inline bool empty() const {
      return m_size == 0;
    }

    inline bool full() const {
      return m_size >= max_size;
    }

    inline value_type *data() {
      return reinterpret_cast<value_type *>(m_memory);
    }

    inline const value_type *data() const {
      return reinterpret_cast<const value_type *>(m_memory);
    }

    inline value_type *begin() {
      return _Data();
    }

    inline const value_type *begin() const {
      return _Data();
    }

    inline value_type *end() {
      return _Data() + m_size;
    }

    inline const value_type *end() const {
      return _Data() + m_size;
    }

    inline value_type &back() {
      return (_Data() - 1)[m_size];
    }

    inline const value_type &back() const {
      return (_Data() - 1)[m_size];
    }

    inline value_type &operator[](const size_type index) {
#if _CONTAINER_DEBUG_LEVEL > 0
      if (index >= m_size)
        throw std::out_of_range("index");
#endif
      return _Data()[index];
    }

    inline const value_type &operator[](const size_type index) const {
#if _CONTAINER_DEBUG_LEVEL > 0
      if (index >= m_size)
        throw std::out_of_range("index");
#endif
      return _Data()[index];
    }

    inline void pop_back() {
      if (m_size == 0)
        throw std::length_error("StaticSpan can't pop back when empty");

      _Destroy_back();
      m_size--;
    }

    inline void clear() {
      for (; m_size; --m_size)
      {
        _Destroy_back();
      }
    }

    template <class... _Valty>
    inline value_type &emplace_back(_Valty&&... _Val) {
      _Ty &_Result = _Emplace_back(std::forward<_Valty>(_Val)...);
      return _Result;
    }

    inline void push_back(const value_type &value) {
      _Emplace_back(value);
    }

    inline void push_back(value_type &&value) {
      _Emplace_back(std::move(value));
    }

    inline void insert(size_t index, const value_type &value) {
      if (m_size == max_size)
        throw std::length_error("StaticSpan insert fail, is full");
      if (index == m_size)
      {
        (void)_Emplace_back(value);
        return;
      }

      _Shift_range({ 0, index }, 1);
      _Construct(&_Data()[index], value);
    }

    inline void insert(size_type index, value_type &&value) {
#if _CONTAINER_DEBUG_LEVEL > 0
      if (index > m_size)
        throw std::out_of_range("index");
      if (m_size == max_size)
        throw std::length_error("StaticSpan insert fail, is full");
#endif // _CONTAINER_DEBUG_LEVEL > 0



      if (index == m_size)
      {
        (void)_Emplace_back(value);
        return;
      }

      _Shift_range({ 0, index }, 1);
      _Construct(&_Data()[index], value);
    }

    inline void erase(size_type index) {
#if _CONTAINER_DEBUG_LEVEL > 0
      if (index >= m_size)
        throw std::out_of_range("index");
#endif // _CONTAINER_DEBUG_LEVEL > 0

      _Shift_range({ index + 1, m_size }, -1);
    }

    inline void resize(size_type new_size) {
      throw std::runtime_error("not implemented");
    }

    inline void fill(const value_type &value) {
      throw std::runtime_error("not implemented");
    }

    inline void fill(value_type &&value) {
      throw std::runtime_error("not implemented");
    }

  private:
    size_t m_size;
    int8_t m_memory[max_size * sizeof(value_type)];
  };


}
