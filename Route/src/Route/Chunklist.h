#pragma once
#include "Bases.h"

namespace route
{

	template <typename _Ty, size_t _ChSz = 32>
	class chunklist
	{
	public:
		using this_type = chunklist<_Ty, _ChSz>;
		using value_type = _Ty;
		using size_type = size_t;
		using size_pair = std::pair<size_type, size_type>;
		static constexpr size_type chunk_capacity = next_pow2( _ChSz );
		static constexpr size_type chunk_max_size = chunk_capacity;
		static constexpr size_type chunk_capacity_shift = floor_log2( chunk_capacity );

		struct chunk
		{
			using mem_elm = uint8_t;
			static constexpr auto val_sz = sizeof( _Ty );
			static constexpr auto mem_len = (val_sz / sizeof( mem_elm )) + (val_sz % sizeof( mem_elm ) ? 1 : 0);

			FORCE_INLINE value_type *operator[]( const size_type index ) {
				return reinterpret_cast<value_type *>(data[ index ]);
			}

			FORCE_INLINE const value_type *operator[]( const size_type index ) const {
				return reinterpret_cast<const value_type *>(data[ index ]);
			}

			size_type len;
			std::array<mem_elm[ mem_len ], mem_len> data;
		};

		static inline constexpr size_type max_size() noexcept {
			return std::numeric_limits<size_type>::max() >> chunk_capacity_shift;
		}

		inline size_type size() const {
			if (m_ptrs.empty())
				return 0;

			// counter of chunks except the last one (last one might not be fully filled)
			// + the actual length of the last chunk
			return (m_ptrs.size() - 1) * chunk_capacity + m_ptrs.back()->len;
		}

		inline bool empty() const {
			return m_ptrs.empty();
		}

		inline value_type &operator[]( const size_type index ) {
			const auto pos = _pos( index );
			chunk *const chunk = m_ptrs[ pos.first ];
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(
				pos.second < chunk->len, "chunklist subscript out of range" );
#endif // _CONTAINER_DEBUG_LEVEL > 0

			return *chunk->operator[]( pos.second );
		}

		inline const value_type &operator[]( const size_type index ) const {
			const auto pos = _pos( index );
			const chunk *const chunk = m_ptrs[ pos.first ];
#if _CONTAINER_DEBUG_LEVEL > 0
			_STL_VERIFY(
				pos.second < chunk->len, "chunklist subscript out of range" );
#endif // _CONTAINER_DEBUG_LEVEL > 0

			return *chunk->operator[]( pos.second );
		}

		inline chunk **data() {
			return m_ptrs.data();
		}

		inline const chunk *const *data() const {
			return m_ptrs.data();
		}

		inline const chunk &_chunk_at( const size_type index ) const {
			return *m_ptrs[ index ];
		}

	private:
		static FORCE_INLINE size_pair _pos( const size_type index ) noexcept {
			static constexpr size_type element_mask = (size_type( 1 ) << chunk_capacity_shift) - 1;
			return { index >> chunk_capacity_shift, index & element_mask };
		}


	private:
		std::vector<chunk *> m_ptrs;
	};

	using val = chunklist<int, 64>;

}
