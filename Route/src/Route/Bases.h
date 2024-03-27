#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <string>
#include <chrono>

namespace route
{
	using std::array;
	using std::vector;
	using std::string;

	typedef float real_t;

	using time_point_ms = std::chrono::time_point<std::chrono::steady_clock, std::chrono::milliseconds>;
	using time_point_us = std::chrono::time_point<std::chrono::steady_clock, std::chrono::microseconds>;
	using time_point_ns = std::chrono::time_point<std::chrono::steady_clock, std::chrono::nanoseconds>;

	typedef size_t index_t;
	_INLINE_VAR constexpr index_t npos = (size_t)-1;

	_INLINE_VAR constexpr real_t Pi = 3.1415926f;
	_INLINE_VAR constexpr real_t Tau = Pi * 2.f;
	_INLINE_VAR constexpr real_t E = 2.7182818f;
	_INLINE_VAR constexpr real_t Epsilon = 1e-4f;

	// is it pure? not a reference nor a const nor a volatile type and not the void type
	template <typename _Ty>
	_INLINE_VAR constexpr bool is_pure_v =
		!(std::is_reference_v<_Ty> || std::is_const_v<_Ty> || std::is_volatile_v<_Ty> || std::is_void_v<_Ty>);

	// is the type related?
	template <typename _To, typename _With>
	_INLINE_VAR constexpr bool is_related_v =
		std::is_base_of_v<_To, _With> || std::is_base_of_v<_With, _To> || std::is_same_v<_To, _With>;

	template <typename _Ty, typename _Ey>
	inline constexpr bool is_any() {
		return std::is_same_v<_Ty, _Ey>;
	}

	template <typename _Ty, typename _Ey, typename... _Vargs>
	inline constexpr bool is_any() {
		return std::is_same_v<_Ty, _Ey> || is_any<_Ty, _Vargs>();
	}

	template <typename _Ty, typename _Ey>
	inline constexpr bool is_all() {
		return std::is_same_v<_Ty, _Ey>;
	}

	template <typename _Ty, typename _Ey, typename... _Vargs>
	inline constexpr bool is_all() {
		return std::is_same_v<_Ty, _Ey> && is_all<_Ty, _Vargs>();
	}

	template <typename _Ty>
	inline static constexpr size_t max_sizeof() {
		return sizeof( _Ty );
	}

	template <typename _Ty, typename... _Types>
	inline static constexpr size_t max_sizeof() {
		return std::max( sizeof( _Ty ), max_sizeof<_Types...>() );
	}

	struct nothing
	{
	};
}
