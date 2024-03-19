#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <string>
#include <stack>

// FIXME: MSVC 1800 version is just a guess, double check later
#if defined(_MSC_VER) && _MSC_VER > 1800
#define _INTERFACE __interface
#else
#define _INTERFACE struct
#endif

namespace route
{
	using std::array;
	using std::vector;
	template <typename _Ty>
	using stack = std::stack<_Ty, vector<_Ty>>;
	using string = std::string;

	typedef size_t index_t;
	_INLINE_VAR constexpr index_t npos = (size_t)-1;

	template <typename _Ty>
	inline static constexpr size_t max_sizeof() {
		return sizeof( _Ty );
	}

	template <typename _Ty, typename... _Types>
	inline static constexpr size_t max_sizeof() {
		return std::max( sizeof( _Ty ), max_sizeof<_Types...>() );
	}
}
