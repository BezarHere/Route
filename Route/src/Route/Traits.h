#pragma once
#include "Transform.h"

namespace route
{
	namespace traits
	{
		class BaseTrait
		{
		private:
			// can't be constructed/deleted
			BaseTrait() = delete;
			~BaseTrait() = delete;
		};

		class Impl2D : BaseTrait
		{
		public:
			using transform = Transform2D;
			using direction = typename transform::vector_type;
			using zindex = int16_t;
		};

		class Impl3D : BaseTrait
		{
		public:
			using transform = Transform3D;
			using direction = typename transform::vector_type;
			using zindex = nothing;
		};
	}
}
