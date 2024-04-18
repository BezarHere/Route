#pragma once
#include "Bases.h"
#include <map>

namespace route
{
	template <typename _Cnt>
	class imap_iterator
	{
		using this_type = imap_iterator<_Cnt>;
		using container_type = _Cnt;
		using value_type = typename container_type::kv_pair;
		using bucket_type = typename container_type::bucket;
		using size_type = typename container_type::size_type;

		inline this_type &operator--() {
			if (--m_pos.first >= m_data[ m_pos.second ].data() + m_data[ m_pos.second ].size())
			{
				--m_pos.second;
				m_pos.first = m_data[ m_pos.second ].end()._Ptr;
			}
			return *this;
		}

		inline this_type &operator++() {
			if (++m_pos.first >= m_data[ m_pos.second ].data() + m_data[ m_pos.second ].size())
			{
				++m_pos.second;
				m_pos.first = m_data[ m_pos.second ].data();
			}
			return *this;
		}

		inline value_type &operator*() {
			return *m_pos.first;
		}

		inline value_type *operator->() {
			return m_pos.first;
		}

	private:
		std::pair<value_type *, size_type> m_pos;
		bucket_type *const m_data;
	};

	template <typename _Ky, size_t _BkCn = 16ULL>
	struct imap_traits
	{
		using key_type = int32_t;
		using compare = std::less<key_type>;
		static constexpr size_t buckets_count = _BkCn;
		static constexpr key_type key_hash_mask = static_cast<key_type>(0xF1AC8614E6014B1CULL); /* weak but cheap */
	};

	/// @brief an ordered map indexed by integers only, useful for non-uniform indexing
	/// @tparam _Ty the value type
	template <typename _Ty, typename _Traits = imap_traits<_Ty>>
	class imap
	{
	public:
		using this_type = imap<_Ty, _Traits>;
		using traits = _Traits;
		using key_type = typename traits::key_type;
		using value_type = _Ty;
		using size_type = size_t;
		using compare = typename traits::compare;
		using kv_pair = std::pair<key_type, value_type>;
		using bucket = vector<kv_pair>;
		static constexpr size_type buckets_count = next_pow2( size_type( traits::buckets_count ) );
		static constexpr size_type buckets_count_shift = floor_log2( buckets_count );

		static_assert(std::is_integral_v<key_type>, "Only supports integral key-types");

		using iterator = imap_iterator<this_type>;
		//using const_iterator = const_imap_iterator;

	private:
		static FORCE_INLINE size_type _bucket_pos( const key_type index ) {
			constexpr key_type mask = (key_type( 1 ) << buckets_count_shift) - 1;
			return ((index >> buckets_count_shift) ^ (_rotl64( index, buckets_count_shift )) ^ traits::key_hash_mask) & mask;
		}

		// binary search
		static inline constexpr
			index_t _search( const kv_pair *first, const size_type length, const key_type key ) {
			// empty or nullptr
			if (!(first && length)) return npos;
			// Smart compilers optimize this (especially if it's an empty object)
			const compare comp{};

			size_type start{};
			size_type window_ln = length;
			while (window_ln > 0)
			{
				const size_type index = start + (window_ln >> 1);
				// half window length
				window_ln >>= 1;

				// Not the target entry
				if (first[ index ].first != key)
				{
					if (!comp( first[ index ].first, key )) // the entry is ahead of us
					{
						// the entry is ahead of us, advance start index
						start = index;
					}

					continue;
				}

				// Found the target entry
				return index;
			}

			// Target entry doesn't exist
			return npos;
		}

		static inline constexpr
			bool _has( const kv_pair *data, const size_type length, const key_type key ) {
			const compare comp{};

			// key is Out Of Range
			// for ex: comp is std::less<key_type>
			// the key is OOR if key is less then the smallest value, or the largest value is less then key
			if (comp( key, data[ 0 ].first ) || comp( data[ length - 1 ].first, key ))
				return false;

			return _search( data, length, key ) != npos;
		}

		// a viable index to insert a new entry with the key `key`
		static inline constexpr
			index_t _index_bound( const kv_pair *data, const size_type length, const key_type key ) {
			// empty data or null data (maybe the bucket hasn't bee initalized)
			if (!data || !length)
				return 0; // insert first entry

			// Smart compilers optimize this (especially if it's an empty object)
			const compare comp{};

			size_type start{};
			size_type window_ln = length;

			// search until the window contains 1 or less elements
			while (window_ln > 0)
			{
				const size_type index = start + (window_ln >> 1);
				window_ln >>= 1;

				// key (may) can be AFTER or AT the current index
				if (!comp( key, data[ index ].first ))
				{
					// the key == key at index
					// duplicates are illagle, return npos
					if (key == data[ index ].first)
						return npos;

					// there is a next index, and the key can be before the next index
					// return next index (pushing any val at i + 1 to i + 2)
					if (index < length - 1 && comp( key, data[ index + 1 ].first ))
						return index + 1;

					// we need to after current entry, but the current entry is the LAST entry
					// push it after the last entry (back of the bucket)
					if (index == length - 1)
						return length; // index + 1

					// can put the key after or at the current index
					// advance window/start pos

					start = index;
					continue;
				}

				// can be BEFORE the current index
				// but index is 0, this key must be the lowest in the data
				// return 0 so entry at index 0 will go to index 1
				if (index == 0)
					return 0;

				// key (may) can be before the current index
				// retreat window (already done after setting `index`)
			}

			// Viable upper bound not found
			return npos;
		}

		inline kv_pair *_find( const key_type key ) {
			auto &bucket = m_buckets[ _bucket_pos( key ) ];
			const size_t index = _search( bucket.data(), bucket.size(), key );
			return index == npos ? nullptr : bucket.data() + index;
		}

		inline const kv_pair *_find( const key_type key ) const {
			const auto &bucket = m_buckets[ _bucket_pos( key ) ];
			const size_t index = _search( bucket.data(), bucket.size(), key );
			return index == npos ? nullptr : bucket.data() + index;
		}

	public:

		inline value_type &at( const key_type key ) {
			kv_pair *const pkv = _find( key );

			if (pkv)
				return pkv->second;

			throw std::out_of_range( "Invalid map<_Ty, _Traits> key" );
		}

		inline const value_type &at( const key_type key ) const {
			const kv_pair *const pkv = _find( key );

			if (pkv)
				return pkv->second;

			throw std::out_of_range( "Invalid map<_Ty, _Traits> key" );
		}

		template <typename... _VA>
		inline value_type &try_emplace( const key_type key, _VA&&... args ) {
			const index_t pos = _bucket_pos( key );
			bucket &bucket = m_buckets[ pos ];
			const index_t insert_index = _index_bound( bucket.data(), bucket.size(), key );

			// either the bucket is un-ordered or something has been fucked
			if (insert_index == npos)
				return at( key );

			return bucket.insert( bucket.begin() + insert_index, { key, value_type{ std::forward<_VA>( args )... } } )->second;
		}

		inline value_type &add( const key_type key, const value_type &value ) {
			const index_t pos = _bucket_pos( key );
			bucket &bucket = m_buckets[ pos ];
			const index_t insert_index = _index_bound( bucket.data(), bucket.size(), key );

			// either the bucket is un-ordered or something has been fucked
			if (insert_index == npos)
				return at( key );

			return bucket.insert( bucket.begin() + insert_index, { key, value } )->second;
		}

		inline value_type &add( const key_type key, value_type &&value ) {
			const index_t pos = _bucket_pos( key );
			bucket &bucket = m_buckets[ pos ];
			const index_t insert_index = _index_bound( bucket.data(), bucket.size(), key );

			// either the bucket is un-ordered or something has been fucked
			if (insert_index == npos)
				std::_Xruntime_error( "Invalid/npos insert index to add" );

			return bucket.insert( bucket.begin() + insert_index, { key, value } )->second;
		}

		// TODO: return iterator
		inline void remove( const key_type key ) {
			const index_t pos = _bucket_pos( key );
			bucket &bucket = m_buckets[ pos ];
			const index_t index = _search( bucket.data(), bucket.size(), key );
			if (index == npos)
				return; /* entry doesn't exist, should we throw an exception? */

			bucket.erase( bucket.begin() + index );
		}

		inline bool contains( const key_type key ) const {
			return _find( key ) != nullptr;
		}

		inline value_type &operator[]( const key_type key ) {
			return;
		}


	private:
		array<bucket, buckets_count> m_buckets;
	};

	using kv_s = imap<string>;

}
