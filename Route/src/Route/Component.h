#pragma once
#include "Bases.h"
#include "Vector.h"
#include "Boxed.h"

namespace route
{
	class BaseObject;

	namespace comps
	{
		_INTERFACE Component
		{
			virtual void added( BaseObject &object ) = 0;
			virtual void removed( BaseObject &object ) = 0;
			virtual void update( BaseObject &object ) = 0;
			virtual void draw( const BaseObject &object ) const = 0;

		};

		struct ShapeComponent : public Component
		{
		};




		static constexpr size_t component_max_size = 128;
	}
	template <typename _Ty>
	_INLINE_VAR constexpr bool is_component_v =
		std::is_same_v<comps::Component, _Ty> || std::is_base_of_v<comps::Component, _Ty>;

	using component_boxed = boxed<comps::Component, placement_block<comps::component_max_size>>;

}
