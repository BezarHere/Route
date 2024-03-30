#pragma once
#include <vector>

namespace route
{
	// doesn't mange or own it's memory span
	template <typename _Ty>
	class array_view
	{
	public:
		using value_type = _Ty;

		inline array_view()
			: m_begin{}, m_end{} {
		}

		inline array_view( value_type *data, const size_t length )
			: m_begin{ data }, m_end{ data + length } {
		}

		inline array_view( value_type *begin, value_type *end )
			: m_begin{ begin }, m_end{ end } {
		}

		template <size_t _Sz>
		inline array_view( value_type( &data )[ _Sz ] )
			: array_view( data, _Sz ) {
		}

		inline value_type *begin() {
			return m_begin;
		}

		inline value_type *end() {
			return m_end;
		}

		inline const value_type *begin() const {
			return m_begin;
		}

		inline const value_type *end() const {
			return m_end;
		}

		inline value_type *data() {
			return m_begin;
		}

		inline const value_type *data() const {
			return m_begin;
		}

		inline size_t size() const {
			return m_end - m_begin;
		}

		inline bool empty() const {
			return m_begin == m_end;
		}

		inline value_type &operator[]( const size_t index ) {
			return m_begin[ index ];
		}

		inline const value_type &operator[]( const size_t index ) const {
			return m_begin[ index ];
		}

	private:
		value_type *m_begin, *m_end;
	};

}
