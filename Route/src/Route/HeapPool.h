#pragma once
#include <array>

template <typename _T, size_t _ChunkSize = std::max<size_t>(sizeof(size_t), sizeof(_T))>
class HeapPool
{
public:
  using value_type = _T;

private:
  enum _ChunkMapFlags : uint8_t
  {
    eChunkMapFlag_None = 0x0,
    eChunkMapFlag_Allocated = 0x1,
    eChunkMapFlag_Owned = 0x2,
  };
  static constexpr size_t ChunkSize = _ChunkSize;

  struct chunk_type {
    uint8_t _[ChunkSize];
  };

  using map_element = _ChunkMapFlags;

  static inline size_t to_chunks_count(size_t objects_count) {
    size_t count = objects_count * std::max(sizeof(value_type), sizeof(chunk_type));

    if (sizeof(chunk_type) % count)
      return count / sizeof(chunk_type) + 1;
    else
      return count / sizeof(chunk_type);
  }

  static inline size_t sizeof_to_chunks_count(size_t size) {
    if (sizeof(chunk_type) % size)
      return size / sizeof(chunk_type) + 1;
    else
      return size / sizeof(chunk_type);
  }

  static inline map_element *create_map(size_t chunks_count) {
    return new map_element[chunks_count]{};
  }

  static inline size_t _search_map(map_element *const map, const size_t length, const size_t count) {
    size_t end = length - count;
    for (size_t i = 0; i <= end; i++)
    {
      if (map[i] & eChunkMapFlag_Allocated)
        continue;

      map_element *const cur_map = map + i;
      bool found = true;
      for (uint32_t j = 1; j < count; j++)
      {
        // found an active (allocated) chunk
        if (cur_map[i] & eChunkMapFlag_Allocated)
        {
          found = false;
          break;
        }
      }

      if (found)
        return i;

    }
    return npos;
  }

  inline size_t _find_space(size_t count) const {
    size_t index = _search_map(m_map + m_current, m_chunks_count - m_current, count);
    if (index != npos)
      return index + m_current;
    return _search_map(m_map, m_current, count);
  }

public:

  inline HeapPool(size_t size)
    : m_size{ size }, m_chunks_count{ to_chunks_count(size) }, m_current{ 0 },
    m_map{ create_map(m_chunks_count) }, m_pool{ new chunk_type[m_chunks_count] } {
  }

  inline HeapPool() : HeapPool(32) {

  }

  inline ~HeapPool() {
    delete[] m_map;
    delete[] m_pool;
  }

  inline void free(const value_type *ptr) {
    if (!ptr)
      return;
    const chunk_type *chunk = reinterpret_cast<const chunk_type *>(ptr);
    if (chunk < m_pool || chunk >= m_pool + m_chunks_count)
      throw std::runtime_error("'ptr' isn't a valid pointer");

    const size_t index = chunk - m_pool;

    if (!(m_map[index] & eChunkMapFlag_Owned))
      throw std::runtime_error("'ptr' is not allocated");

    // mark empty
    m_map[index] = eChunkMapFlag_None;
    for (size_t i = index + 1; i < m_chunks_count; i++)
    {
      // until we found another chunk or an empty space
      if (m_map[i] & eChunkMapFlag_Owned || m_map[i] == eChunkMapFlag_None)
        break;
      m_map[i] = eChunkMapFlag_None;
    }
  }

  inline value_type *alloc(const size_t size = 1) {
    const size_t count = sizeof_to_chunks_count(size);
    size_t index = _find_space(count);
    if (index == -1)
      return nullptr;

    m_map[index] = static_cast<map_element>(eChunkMapFlag_Allocated | eChunkMapFlag_Owned);
    for (size_t i = 1; i < count; i++)
      m_map[index + i] = static_cast<map_element>(eChunkMapFlag_Allocated);

    m_current = index + count;

    return reinterpret_cast<value_type *>(&m_pool[index]);
  }

  template <typename _E, typename... _Args>
  inline value_type *create(_Args&&...args) {
    value_type *ptr = alloc(sizeof(_E));
    if (ptr == nullptr)
      std::_Xbad_alloc();
    new(ptr) _E(std::forward(args)...);
    return ptr;
  }

  template <typename _E>
  inline void destroy(_E *object) {

    if constexpr (!std::is_void_v<_E> && !std::is_trivial_v<_E>)
    {
      if (object == nullptr)
        return;
      object->~_E();
    }
    free(reinterpret_cast<const value_type *>(object));
  }

  // amount of objects of size _T that this pool can hold
  inline size_t size() const {
    return m_size;
  }

private:
  HeapPool(HeapPool &&) = delete;
  HeapPool(const HeapPool &) = delete;
  HeapPool &operator=(HeapPool &&) = delete;
  HeapPool &operator=(const HeapPool &) = delete;

private:
  const size_t m_size;
  const size_t m_chunks_count;
  size_t m_current;
  map_element *const m_map;
  chunk_type *const m_pool;
};

