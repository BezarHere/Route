#pragma once
#include <array>

namespace route
{
	template <typename _Ty>
	struct type_shifter
	{
		using value_type = _Ty;

		template <typename _Ey>
		static void construct( value_type *to, const _Ey &from ) {
			new (to) _Ey( from );
		}

		template <typename _Ey>
		static void construct( value_type *to, _Ey &&from ) {
			new (to) _Ey( from );
		}

		static void destroy( value_type &obj ) {
			obj.~value_type();
		}

		static void assign( value_type &to, const value_type &from ) {
			to = from;
		}

		static void assign( value_type &to, value_type &&from ) {
			to = from;
		}

	};

	template <typename _Ty, size_t _Sz = sizeof(_Ty), typename _Shift = type_shifter<_Ty>>
	class boxed
	{
	public:
		static_assert(sizeof( _Ty ) <= _Sz, "Size is too small for _Ty");

		using this_type = boxed<_Ty, _Sz, _Shift>;
		using value_type = _Ty;
		using type_shifter = _Shift;
		static constexpr size_t size = _Sz;
		using memory_element = uint8_t;
		static constexpr size_t memory_count =
			size / sizeof( memory_element ) + ((size % sizeof( memory_element )) ? 1 : 0);
		using block_type = array<memory_element, memory_count>;
		static constexpr bool abstract = std::is_abstract_v<_Ty>;

		template <typename _Ey, typename... _Vargs>
		static inline this_type make( _Vargs&& ...args ) {
			this_type val{};
			new (val.m_memory.data()) _Ey( std::forward<_Vargs>( args )... );
			return val;
		}

		inline value_type *ptr() noexcept {
			return reinterpret_cast<value_type *>(m_memory.data());
		}

		inline value_type *data() noexcept {
			return reinterpret_cast<value_type *>(m_memory.data());
		}

		inline const value_type *ptr() const noexcept {
			return reinterpret_cast<const value_type *>(m_memory.data());
		}

		inline const value_type *data() const noexcept {
			return reinterpret_cast<const value_type *>(m_memory.data());
		}

		inline boxed() = default;
		inline boxed( const this_type &copy );
		inline boxed( this_type &&move ) noexcept;
		inline this_type &operator=( const this_type &copy );
		inline this_type &operator=( this_type &&move ) noexcept;


		inline boxed( const value_type &copy ) {
			if constexpr (abstract)
				std::_Xruntime_error( "abstract types can't be copy-constructible; try using boxed<T>::make" );
			else
				type_shifter::construct( ptr(), copy );
		}

		inline boxed( value_type &&move ) noexcept {
			if constexpr (abstract)
				std::_Xruntime_error( "abstract types can't be move-constructible; try using boxed<T>::make" );
			else
				type_shifter::construct( ptr(), move );
		}

		inline this_type &operator=( const value_type &copy ) {
			type_shifter::assign( *ptr(), copy );
			return *this;
		}

		inline this_type &operator=( value_type &&copy ) {
			type_shifter::assign( *ptr(), copy );
			return *this;
		}

		inline ~boxed() {
			type_shifter::destroy( *ptr() );
		}

		inline const value_type &operator*() const noexcept(!std::is_abstract_v<value_type>) {
			if constexpr (std::is_abstract_v<value_type>)
			{
				throw std::runtime_error( "Can't const dereference an abstract type; use get<DerivedType>()" );
			}
			else
			{
				return *static_cast<const value_type *>(m_memory.data());
			}
		}

		inline value_type &operator*() noexcept(!std::is_abstract_v<value_type>) {
			if constexpr (std::is_abstract_v<value_type>)
			{
				throw std::runtime_error( "Can't dereference an abstract type; use get<DerivedType>()" );
			}
			else
			{
				return *static_cast<value_type *>(m_memory.data());
			}
		}

		inline const value_type *operator->() const noexcept {
			return static_cast<const value_type *>(m_memory.data());
		}

		inline value_type *operator->() noexcept {
			return static_cast<value_type *>(m_memory.data());
		}

		// derived get
		template <typename _Ey>
		inline _Ey &get() noexcept {
			static_assert(std::is_base_of_v<_Ty, _Ey>, "Can't cast to non-derived");
			return *dynamic_cast<_Ey *>(m_memory.data());
		}

		// derived get
		template <typename _Ey>
		inline const _Ey &get() const noexcept {
			static_assert(std::is_base_of_v<_Ty, _Ey>, "Can't cast to non-derived");
			return *dynamic_cast<_Ey *>(m_memory.data());
		}

	private:
		block_type m_memory;
	};

	template <typename _Ty, size_t _Sz, typename _Shift>
	inline boxed<_Ty, _Sz, _Shift>::boxed( const this_type &copy ) : boxed( *copy ) {
	}

	template <typename _Ty, size_t _Sz, typename _Shift>
	inline boxed<_Ty, _Sz, _Shift>::boxed( this_type &&move ) noexcept : boxed( *move ) {
	}

	template <typename _Ty, size_t _Sz, typename _Shift>
	inline boxed<_Ty, _Sz, _Shift> &boxed<_Ty, _Sz, _Shift>::operator=( const this_type &copy ) {
		return operator=( *copy );
	}

	template <typename _Ty, size_t _Sz, typename _Shift>
	inline boxed<_Ty, _Sz, _Shift> &boxed<_Ty, _Sz, _Shift>::operator=( this_type &&move ) noexcept {
		return operator=( *move );
	}

}
