#pragma once
#include <array>

namespace route
{
	template <typename _Ty, size_t _Sz = sizeof( _Ty )>
	class boxed
	{
	public:
		static_assert(sizeof( _Ty ) <= _Sz, "Size is too small for _Ty");

		using this_type = boxed<_Ty, _Sz>;
		using value_type = _Ty;
		static constexpr size_t size = _Sz;
		using memory_element = uint8_t;
		static constexpr memory_count =
			size / sizeof( memory_element ) + ((size % sizeof( memory_element )) ? 1 : 0);
		using block_type = array<memory_element, memory_count>;
		static constexpr bool abstract = std::is_abstract_v<_Ty>;

		template <typename _Ey, typename... _Vargs>
		static inline this_type make( _Vargs&& ...args ) {
			this_type val{};
			new (val.m_memory.get_ptr()) _Ey( std::forward<_Vargs>( args )... );
			return val;
		}

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

		inline boxed() = default;
		inline boxed( const this_type &copy );
		inline boxed( this_type &&move ) noexcept;
		inline this_type &operator=( const this_type &copy );
		inline this_type &operator=( this_type &&move ) noexcept;


		inline boxed( const value_type &copy ) {
			if constexpr (abstract)
			{
				this->operator=( copy );
				//std::_Xruntime_error( "abstract types can't be copy-constructible; try using boxed<T>::make" );
			}
			else
			{
				new(ptr()) value_type( copy );
			}
		}

		inline boxed( value_type &&move ) noexcept {
			if constexpr (abstract)
			{
				this->operator=( move );
				//std::_Xruntime_error( "abstract types can't be move-constructible; try using boxed<T>::make" );
			}
			else
			{
				new(ptr()) value_type( move );
			}
		}

		inline this_type &operator=( const value_type &copy ) {
			*ptr() = copy;
			return *this;
		}

		inline this_type &operator=( value_type &&copy ) {
			*ptr() = copy;
			return *this;
		}

		inline ~boxed() {
			ptr()->~value_type();
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
				return *static_cast<value_type *>(m_memory.get_ptr());
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

	template<typename _Ty, size_t _Sz>
	inline boxed<_Ty, _Sz>::boxed( const this_type &copy ) : boxed( *copy ) {
	}

	template<typename _Ty, size_t _Sz>
	inline boxed<_Ty, _Sz>::boxed( this_type &&move ) noexcept : boxed( *move ) {
	}

	template<typename _Ty, size_t _Sz>
	inline boxed<_Ty, _Sz> &boxed<_Ty, _Sz>::operator=( const this_type &copy ) {
		return operator=( *copy );
	}

	template<typename _Ty, size_t _Sz>
	inline boxed<_Ty, _Sz> &boxed<_Ty, _Sz>::operator=( this_type &&move ) noexcept {
		return operator=( *move );
	}

}
