#pragma once
#include <stdint.h>
#include <array>
#include <vector>
#include <string>
#include <chrono>

#ifndef FORCE_INLINE
#if defined(__GNUC__) || defined(__clang__)
#define FORCE_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define FORCE_INLINE __forceinline
#else
#define FORCE_INLINE inline
#endif
#endif

#if ((defined(__GNUC__) && (__GNUC__ >= 2)) || defined(_MSC_VER) || defined (__WATCOMC__))
#		define RT_FUNCNAME __FUNCTION__
#else
#		define RT_FUNCNAME "UNKNOWN"
#endif

#define RT_FILE    __FILE__
#define RT_LINE    __LINE__

#define _RT_PRINT_ASSERT(str) fprintf(stderr, "Failed to assert '%s', at " RT_FILE "::" RT_FUNCNAME "::%d\n", str, RT_LINE);


#define _RT_ASSERT_ON(expr) if (!(expr)) { _RT_PRINT_ASSERT(#expr); }
#define _RT_ASSERT_OFF(expr) (void)sizeof(expr)

#ifndef RT_ASSERT_LVL
#	ifdef _DEBUG
#		define RT_ASSERT_LVL 2
#	else
#		define RT_ASSERT_LVL 1
#	endif
#endif

/* Note: the 'JIC' in RT_ASSERT_JIC stands for Just-In-Case */

#if RT_ASSERT_LVL == 0
#	define RT_ASSERT _RT_ASSERT_OFF
#	define RT_ASSERT_RELEASE _RT_ASSERT_OFF
#	define RT_ASSERT_JIC _RT_ASSERT_OFF
#elif RT_ASSERT_LVL == 1
#	define RT_ASSERT _RT_ASSERT_OFF
#	define RT_ASSERT_RELEASE _RT_ASSERT_ON
#	define RT_ASSERT_JIC _RT_ASSERT_OFF
#elif RT_ASSERT_LVL == 2
#	define RT_ASSERT _RT_ASSERT_ON
#	define RT_ASSERT_RELEASE _RT_ASSERT_ON
#	define RT_ASSERT_JIC _RT_ASSERT_OFF
#elif RT_ASSERT_LVL == 3
#	define RT_ASSERT _RT_ASSERT_ON
#	define RT_ASSERT_RELEASE _RT_ASSERT_ON
#	define RT_ASSERT_JIC _RT_ASSERT_ON
#else
#error ROUTE: UNKNWON ASSERT LEVEL
#endif

namespace route
{
	using std::array;
	using std::vector;
	using std::string;

	template <typename _Ty>
	using pair = std::pair<_Ty, _Ty>;

	typedef float real_t;

	using std::chrono::milliseconds;
	using std::chrono::microseconds;
	using std::chrono::nanoseconds;

	using time_point_ms = std::chrono::time_point<std::chrono::steady_clock, milliseconds>;
	using time_point_us = std::chrono::time_point<std::chrono::steady_clock, microseconds>;
	using time_point_ns = std::chrono::time_point<std::chrono::steady_clock, nanoseconds>;



	typedef size_t index_t;
	typedef uintptr_t refc_t;
	typedef uintptr_t vpid_t; /* virtual pointer id, can substitute a pointer or an index in a state map */
	_INLINE_VAR constexpr index_t npos = (size_t)-1;

	_INLINE_VAR constexpr real_t Pi = 3.1415926f;
	_INLINE_VAR constexpr real_t Tau = Pi * 2.f;
	_INLINE_VAR constexpr real_t E = 2.7182818f;
	_INLINE_VAR constexpr real_t Epsilon = 1e-4f;

	template <typename _Ty>
	inline _Ty *memcpy( _Ty *dst, const _Ty *src, const size_t count ) {
		return reinterpret_cast<_Ty *>(std::memcpy( dst, src, count * sizeof( _Ty ) ));
	}

	template <typename _Ty>
	inline _Ty *memset( _Ty *dst, const int val, const size_t count ) {
		return reinterpret_cast<_Ty *>(std::memset( dst, val, count * sizeof( _Ty ) ));
	}

	template <typename _Ty>
	inline constexpr std::size_t hash_type() {
#ifdef _MSC_VER
		constexpr char FN_SIG[] = __FUNCSIG__;
#else
#define FN_SIG __PRETTY_FUNCTION__
#endif
		std::size_t result{};

		for (const char c : FN_SIG)
		{
			result += (c & 1) ? (c * 16777619ULL) : (~c * 2166136261ULL);
		}

		return result;
#undef FN_SIG
	}

	// is it pure? not a reference nor a const nor a volatile type and not the void type
	template <typename _Ty>
	_INLINE_VAR constexpr bool is_pure_v =
		!(std::is_reference_v<_Ty> || std::is_const_v<_Ty> || std::is_volatile_v<_Ty> || std::is_void_v<_Ty>);

	// is the type related?
	template <typename _To, typename _With>
	_INLINE_VAR constexpr bool is_related_v =
		std::is_base_of_v<_To, _With> || std::is_base_of_v<_With, _To> || std::is_same_v<_To, _With>;

	// first type in a Variable args
	template <typename _Ty, typename... _Vargs>
	using first_v = _Ty;

	template <typename _Ty, typename _Ey>
	inline constexpr bool is_any() {
		return std::is_same_v<_Ty, _Ey>;
	}

	template <typename _Ty, typename _Ey, typename _Ey2, typename... _Vargs>
	inline constexpr bool is_any() {
		return std::is_same_v<_Ty, _Ey> || is_any<_Ty, _Ey2, _Vargs...>();
	}

	template <typename _Ty, typename _Ey>
	inline constexpr bool is_all() {
		return std::is_same_v<_Ty, _Ey>;
	}

	template <typename _Ty, typename _Ey, typename _Ey2, typename... _Vargs>
	inline constexpr bool is_all() {
		return std::is_same_v<_Ty, _Ey> &&is_all<_Ty, _Ey2, _Vargs...>();
	}

	template <typename _Ty, typename... _Vargs>
	_INLINE_VAR constexpr auto is_any_v = is_any<_Ty, _Vargs...>();

	template <typename _Ty, typename... _Vargs>
	_INLINE_VAR constexpr auto is_all_v = is_all<_Ty, _Vargs...>();

	template <typename _Ty>
	_INLINE_VAR constexpr auto is_index_v = is_any_v<_Ty, uint8_t, uint16_t, uint32_t, uint64_t>;

	template <typename _Ty>
	inline static constexpr size_t max_sizeof() {
		return sizeof( _Ty );
	}

	template <typename _Ty, typename _Ey, typename... _Types>
	inline static constexpr size_t max_sizeof() {
		return std::max( sizeof( _Ty ), max_sizeof<_Ey, _Types...>() );
	}

	// From The Godot Game Engine, modified by zahr abdullatif
	// Function to find the next power of 2 to an integer
	template <typename _Ty>
	FORCE_INLINE constexpr _Ty next_pow2( _Ty x ) {
		if (x == 0)
		{
			return 0;
		}

		--x;

		for (uint32_t i = 1; i < (sizeof( _Ty ) * 8); i <<= 1)
			x |= x >> i;

		return ++x;
	}

	// From The Godot Game Engine, modified by zahr abdullatif
	// Function to find the previous power of 2 to an integer.
	template <typename _Ty>
	FORCE_INLINE constexpr _Ty prev_pow2( _Ty x ) {
		for (uint32_t i = 1; i < (sizeof( _Ty ) * 8); i <<= 1)
			x |= x >> i;
		return x - (x >> 1);
	}

	// From The Godot Game Engine, modified by zahr abdullatif
	// Function to find the closest power of 2 to an integer.
	template <typename _Ty>
	FORCE_INLINE constexpr _Ty closest_pow2( const _Ty x ) {
		const _Ty nx = next_pow2( x );
		const _Ty px = prev_pow2( x );
		return (nx - x) > (x - px) ? px : nx;
	}

	// Function to find the closest power of 2 to an integer.
	template <typename _Ty>
	FORCE_INLINE constexpr _Ty floor_log2( _Ty x ) {
		for (int16_t i = (sizeof( _Ty ) * 8) - 1; i >= 0; i--)
		{
			if (x & (_Ty( 1 ) << i))
				return i;
		}
		return 0;
	}

	template <typename _Ty>
	constexpr _Ty idiv_ceil( _Ty numerator, _Ty denominator ) {
		return numerator / denominator + (numerator % denominator ? 1 : 0);
	}

	template <typename _Ty>
	constexpr _Ty idiv_round( _Ty numerator, _Ty denominator ) {
		return numerator / denominator + ((numerator % denominator >= (denominator >> 1)) ? 1 : 0);
	}


	template <typename _Ty>
	struct UniqueBlob
	{
		size_t length;
		std::unique_ptr<_Ty[]> data;
	};

	template <typename _Ty>
	struct Blob
	{
		inline Blob() {
		}

		template <size_t _Sz>
		inline Blob( _Ty p_data[ _Sz ] ) : length{ _Sz }, data{ p_data } {
		}

		inline Blob( _Ty *p_data, size_t len ) : length{ len }, data{ p_data } {
		}

		size_t length = 0;
		_Ty *data = nullptr;
	};

	template <typename _Ty, size_t _Sz>
	struct InlinedBlob
	{
		size_t length;
		_Ty data[ _Sz ];
	};

	struct nothing
	{
	};
}
