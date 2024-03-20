#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <string>
#include <chrono>

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
	using std::string;

	using time_point_ms =  std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>;
	using time_point_us =  std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
	using time_point_ns =  std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

	typedef size_t index_t;
	_INLINE_VAR constexpr index_t npos = (size_t)-1;

	_INLINE_VAR constexpr float Pi = 3.1415926f;
	_INLINE_VAR constexpr float Tau = Pi * 2.f;
	_INLINE_VAR constexpr float E = 2.7182818f;

	_INLINE_VAR constexpr double dPi = 3.141592653589793;
	_INLINE_VAR constexpr double dTau = dPi * 2.0;
	_INLINE_VAR constexpr double dE = 2.718281828459045;

	_INLINE_VAR constexpr float Epsilon = 1e-4f;
	_INLINE_VAR constexpr double dEpsilon = 1e-7f;

	template <typename _Ty>
	inline static constexpr size_t max_sizeof() {
		return sizeof( _Ty );
	}

	template <typename _Ty, typename... _Types>
	inline static constexpr size_t max_sizeof() {
		return std::max( sizeof( _Ty ), max_sizeof<_Types...>() );
	}
}
