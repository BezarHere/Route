#pragma once
#include <array>

namespace route
{

	template <size_t _Sz>
	struct placement_block
	{
		using element_type = size_t;
		static constexpr size_t size = _Sz;
		static constexpr size_t block_size = size / sizeof( element_type ) + (size % sizeof( element_type ) ? 1 : 0);
		using block_type = std::array<element_type, block_size>;

		inline const void *get_ptr() const noexcept {
			return static_cast<const void *>(block.data());
		}

		inline void *get_ptr() noexcept {
			return block.data();
		}

		block_type block;
	};

	template <typename _Ty, typename _Mem = placement_block< sizeof( _Ty ) >>
	class boxed
	{
	public:
		using this_type = boxed<_Ty, _Mem>;
		using value_type = _Ty;
		using memory_type = _Mem;
		using block_type = typename memory_type::block_type;

		inline value_type *ptr() noexcept {
			return static_cast<value_type *>(m_memory.get_ptr());
		}

		inline value_type *data() noexcept {
			return static_cast<value_type *>(m_memory.get_ptr());
		}

		inline const value_type *ptr() const noexcept {
			return static_cast<const value_type *>(m_memory.get_ptr());
		}

		inline const value_type *data() const noexcept {
			return static_cast<const value_type *>(m_memory.get_ptr());
		}

		template <typename... _Vargs>
		inline boxed( _Vargs &&...args ) {
			(void)new(m_memory.get_ptr()) value_type( std::forward<_Vargs>( args )... );
		}

		inline ~boxed() {
			if constexpr (!std::is_trivially_destructible_v<value_type>)
			{
				ptr()->~_Ty();
			}
		}

		inline this_type &operator=( const value_type &copy ) {
			return *this = copy;
		}

		inline this_type &operator=( value_type &&copy ) {
			return *this = copy;
		}

		inline const value_type &operator*() const noexcept {
			return *static_cast<const value_type *>(m_memory.get_ptr());
		}

		inline value_type &operator*() noexcept {
			return *static_cast<value_type *>(m_memory.get_ptr());
		}

		inline const value_type *operator->() const noexcept {
			return static_cast<const value_type *>(m_memory.get_ptr());
		}

		inline value_type *operator->() noexcept {
			return static_cast<value_type *>(m_memory.get_ptr());
		}

	private:
		memory_type m_memory;
	};


}
