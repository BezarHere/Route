#pragma once
#include "Bases.h"

namespace route
{
	// passing this to the Memblock will make it copyable/movable;
	// copy & move operators will ONLY exchange the memory content.
	using Flag_TrivialMemblock = void;

	template <typename ..._Va>
	struct Memblock
	{
	public:
		static constexpr size_t size = route::max_sizeof<_Va...>();
		using memory_element = intptr_t;
		static constexpr size_t memory_block_size = size / sizeof( memory_element ) + (size % sizeof( memory_element ) ? 1 : 0);
		using memory_block = array<memory_element, memory_block_size>;

		// checks if the any template parameter is void
		static constexpr bool trivial = route::is_any_v<Flag_TrivialMemblock, _Va...>;

		inline Memblock() : memory{} {
		}

		inline Memblock( const Memblock &copy ) : memory{ copy.memory } {
			static_assert(trivial, "Non-trivial memory block, use the default constructor and properly construct your managed memory!");
		}

		inline Memblock( Memblock &&move ) : memory{ move.memory } {
			static_assert(trivial, "Non-trivial memory block, use the default constructor and properly construct your managed memory!");
		}

		inline Memblock &operator=( const Memblock &copy ) {
			static_assert(trivial, "Non-trivial memory block, define a specialized assign operator for your managed memory!");
			memory = copy.memory;
		}

		inline Memblock &operator=( Memblock &&move ) {
			static_assert(trivial, "Non-trivial memory block, define a specialized assign operator for your managed memory!");
			memory = move.memory;
		}

		template <typename _Ty = first_v<_Va...>>
		FORCE_INLINE _Ty &get() {
			static_assert(route::is_any_v<_Ty, _Va...>, "_Ty isn't any of the types passed in _Va");
			return *reinterpret_cast<_Ty *>(memory.data());
		}

		template <typename _Ty = first_v<_Va...>>
		FORCE_INLINE const _Ty &get() const {
			static_assert(route::is_any_v<_Ty, _Va...>, "_Ty isn't any of the types passed in _Va");
			return *reinterpret_cast<const _Ty *>(memory.data());
		}

		template <typename _Ty = first_v<_Va...>>
		FORCE_INLINE _Ty *data() {
			static_assert(route::is_any_v<_Ty, _Va...>, "_Ty isn't any of the types passed in _Va");
			return reinterpret_cast<_Ty *>(memory.data());
		}

		template <typename _Ty = first_v<_Va...>>
		FORCE_INLINE const _Ty *data() const {
			static_assert(route::is_any_v<_Ty, _Va...>, "_Ty isn't any of the types passed in _Va");
			return reinterpret_cast<const _Ty *>(memory.data());
		}

		memory_block memory;
	};
}
