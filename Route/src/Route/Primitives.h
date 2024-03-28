#pragma once
#include "Boxed.h"

namespace route
{
	enum class PrimitiveType2D
	{
		Rect,
		Circle,
		Triangle,
		Line,
	};

	enum class PrimitiveType3D
	{
		Cube,
		Sphere,
		Pyrimid,
		Plane,
		Tupe,
	};

	template <typename _XForm>
	struct TPrimitiveND
	{
		template <typename>
		struct traits;

		template <>
		struct traits<Transform2D> {
			using type = PrimitiveType2D;
		};

		template <>
		struct traits<Transform3D> {
			using type = PrimitiveType3D;
		};

		using this_type = TPrimitiveND<_XForm>;
		using transform_type = _XForm;
		using vector_type = typename _XForm::vector_type;
		using real_type = typename vector_type::value_type;
		using type_type = typename traits<_XForm>::type; /* ridiculous */

		// more typing
		inline TPrimitiveND( const type_type p_type ) : type{ p_type } {
		}

		// shouldn't be changed, setting it as const gives a severe headache
		/*const*/ type_type type;
	};

	using Primitive2D = TPrimitiveND<Transform2D>;
	using Primitive3D = TPrimitiveND<Transform3D>;

	struct RectPrimitive : public Primitive2D
	{
		inline RectPrimitive()
			: Primitive2D( Primitive2D::type_type::Circle ) {
		}

		inline RectPrimitive( const Rect2f &p_rect )
			: Primitive2D( Primitive2D::type_type::Circle ), rect{ p_rect } {
		}

		Rect2f rect;
	};

	struct CirclePrimitive : public Primitive2D
	{
		inline CirclePrimitive()
			: Primitive2D( Primitive2D::type_type::Circle ) {
		}

		inline CirclePrimitive( const real_type p_radius )
			: Primitive2D( Primitive2D::type_type::Circle ), radius{ p_radius } {
		}

		real_type radius;
	};

	template <typename _Ty>
	struct primitive_type_shifter
	{
		using value_type = _Ty;

		template <typename _Ey>
		static inline  void construct( value_type *to, const _Ey &from ) {
			new (to) _Ey( from );
		}

		template <typename _Ey>
		static inline  void construct( value_type *to, _Ey &&from ) {
			new (to) _Ey( from );
		}

		static inline  void destroy( value_type &obj ) {
			obj.~value_type();
		}

		static inline void assign( value_type &to, const value_type &from );

	};

	using boxed_primitive_2d = boxed<Primitive2D, 64, primitive_type_shifter<Primitive2D>>;
	using boxed_primitive_3d = boxed<Primitive3D, 128, primitive_type_shifter<Primitive3D>>;

	template<>
	inline void primitive_type_shifter<Primitive2D>::assign( value_type &to, const value_type &from ) {
		switch (to.type)
		{
		case PrimitiveType2D::Rect:
			static_cast<RectPrimitive &>(to) = static_cast<const RectPrimitive &>(from);
			return;
		case PrimitiveType2D::Circle:
			static_cast<CirclePrimitive &>(to) = static_cast<const CirclePrimitive &>(from);
			return;
		case PrimitiveType2D::Triangle:

			return;
		case PrimitiveType2D::Line:

			return;
		default:
			to = from;
			return;
		}
	}

	template<>
	inline void primitive_type_shifter<Primitive3D>::assign( value_type &to, const value_type &from ) {
		switch (to.type)
		{
		case PrimitiveType3D::Cube:

			return;
		case PrimitiveType3D::Sphere:

			return;
		case PrimitiveType3D::Pyrimid:

			return;
		case PrimitiveType3D::Plane:

			return;
		case PrimitiveType3D::Tupe:

			return;
		default:
			to = from;
			return;
		}
	}

}
