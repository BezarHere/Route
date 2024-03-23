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
		static_assert(is_pure_v<_Ty>, "_Ty: Only pure types allowed");

		using this_type = boxed<_Ty, _Mem>;
		using value_type = std::remove_cv_t<_Ty>;
		using memory_type = _Mem;
		using block_type = typename memory_type::block_type;
		static constexpr size_t size = memory_type::size;
		static constexpr bool abstract = std::is_abstract_v<_Ty>;

		template <typename _Ey, typename... _Vargs>
		static inline this_type make( _Vargs&& ...args ) {
			this_type val{};
			new (val.m_memory.get_ptr()) _Ey( std::forward<_Vargs>( args )... );
			return val;
		}

		inline boxed() = default;
		inline boxed( const this_type &copy );
		inline boxed( this_type &&move ) noexcept;
		inline this_type &operator=( const this_type &copy );
		inline this_type &operator=( this_type &&move ) noexcept;

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

		inline ~boxed() {
			// FIXME: container type's proxies break
			ptr()->~value_type();
		}

		inline this_type &operator=( const value_type &copy ) {
			return *this = copy;
		}

		inline this_type &operator=( value_type &&copy ) {
			return *this = copy;
		}

		inline const value_type &operator*() const noexcept(!std::is_abstract_v<value_type>) {
			if constexpr (std::is_abstract_v<value_type>)
			{
				throw std::runtime_error( "Can't const dereference an abstract type; use get<DerivedType>()" );
			}
			else
			{
				return *static_cast<const value_type *>(m_memory.get_ptr());
			}
		}

		inline value_type &operator*() noexcept(!std::is_abstract_v<value_type>) {
			if constexpr (std::is_abstract_v<value_type>)
			{
				throw std::runtime_error( "Can't dereference an abstract type; use get<DerivedType>()" );
			}
			else
			{
				return *static_cast<const value_type *>(m_memory.get_ptr());
			}
		}

		inline const value_type *operator->() const noexcept {
			return static_cast<const value_type *>(m_memory.get_ptr());
		}

		inline value_type *operator->() noexcept {
			return static_cast<value_type *>(m_memory.get_ptr());
		}

		// derived get
		template <typename _Ey>
		inline _Ey &get() noexcept {
			static_assert(std::is_base_of_v<_Ty, _Ey>, "Can't cast to non-derived");
			return *dynamic_cast<_Ey *>(m_memory.get_ptr());
		}

		// derived get
		template <typename _Ey>
		inline const _Ey &get() const noexcept {
			static_assert(std::is_base_of_v<_Ty, _Ey>, "Can't cast to non-derived");
			return *dynamic_cast<_Ey *>(m_memory.get_ptr());
		}

	private:
		memory_type m_memory;
	};

	template<typename _Ty, typename _Mem>
	inline boxed<_Ty, _Mem>::boxed( const this_type &copy ) : boxed(*copy) {
	}

	template<typename _Ty, typename _Mem>
	inline boxed<_Ty, _Mem>::boxed( this_type &&move ) : boxed( *move ) noexcept {
	}

	template<typename _Ty, typename _Mem>
	inline boxed<_Ty, _Mem> &boxed<_Ty, _Mem>::operator=( const this_type &copy ) {
		// TODO: insert return statement here
	}

	template<typename _Ty, typename _Mem>
	inline boxed<_Ty, _Mem> &boxed<_Ty, _Mem>::operator=( this_type &&move ) noexcept {
		// TODO: insert return statement here
	}

}
