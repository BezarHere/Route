#pragma once
#include "Bases.h"
#include "Traits.h"
#include "Vector.h"
#include "IMap.h"

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

	protected:
		direction_type offset;
	};

	using Component2D = TSpaceComponent<traits::Impl2D>;
	using Component3D = TSpaceComponent<traits::Impl3D>;

	struct Shape2DComp : public Component2D
	{

		inline void added( Object &object ) override {
		}

		inline void removed( Object &object ) override {
		}

		inline void update( Object &object ) override {
		}


	};

	template <typename _Ty>
	_INLINE_VAR constexpr bool is_component_v =
		std::is_same_v<Component, _Ty> || std::is_base_of_v<Component, _Ty>;
	using component = Component;


}
