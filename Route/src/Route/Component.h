#pragma once
#include "Bases.h"
#include "Traits.h"
#include "Vector.h"
#include "IMap.h"
#include "Primitives.h"
#include "Resource.h"

namespace route
{
	class Object;

	struct Component
	{
		virtual bool compatible_with( const Object &object ) const = 0;

		virtual void added( Object &object ) = 0;
		virtual void removed( Object &object ) = 0;
		virtual void update( Object &object ) = 0;

		virtual ~Component() {
		};

	};

	template <typename _Traits>
	struct TSpaceComponent : Component
	{
	public:
		using transform_type = typename _Traits::transform;
		using direction_type = typename _Traits::direction;

		static bool has_same_space( const Object &obj );

		inline bool compatible_with( const Object &object ) const override {
			return has_same_space( object );
		}

		inline void set_offset( const direction_type &offset ) {
			m_offset = offset;
		}

		inline const direction_type &get_offset() {
			return m_offset;
		}

	protected:
		direction_type m_offset;
	};

	template <typename _Traits>
	struct TDrawenComponent : TSpaceComponent<_Traits>
	{
	public:

		

	protected:
		RID m_vbuf;
		RID m_ibuf;
	};

	using Component2D = TSpaceComponent<traits::Impl2D>;
	using Component3D = TSpaceComponent<traits::Impl3D>;

	template <typename _Ty>
	struct ShapeComp : public Component2D
	{
	public:
		using primitive_boxed_type = _Ty;

		inline ShapeComp() { }

		void added( Object &object ) override;

		void removed( Object &object ) override;

		void update( Object &object ) override;

		primitive_boxed_type primitive;
	};

	using Shape2DComp = ShapeComp<boxed_primitive_2d>;
	using Shape3DComp = ShapeComp<boxed_primitive_3d>;

	template <typename _Ty>
	_INLINE_VAR constexpr bool is_component_v =
		std::is_same_v<Component, _Ty> || std::is_base_of_v<Component, _Ty>;
	using component = Component;


	template<typename _Ty>
	inline void ShapeComp<_Ty>::added( Object &object ) {
	}

	template<typename _Ty>
	inline void ShapeComp<_Ty>::removed( Object &object ) {
	}

	template<typename _Ty>
	inline void ShapeComp<_Ty>::update( Object &object ) {
	}

}
