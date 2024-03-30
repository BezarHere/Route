#pragma once
#include "Bases.h"
#include "Vector.h"

namespace route
{
	class Object;

	struct Component
	{
		virtual void added( Object &object ) = 0;
		virtual void removed( Object &object ) = 0;
		virtual void update( Object &object ) = 0;

		virtual ~Component() {
		};

	};

	struct ShapeComponent : public Component
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
