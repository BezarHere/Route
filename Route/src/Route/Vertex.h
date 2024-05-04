#pragma once
#include "Vector.h"
#include "Color.h"

namespace route
{
	template <typename _Ty, typename _Uv = Vec2us>
	struct TVertex
	{
		_Ty pos;
		_Uv uv;
	};

	template <typename _Ty, typename _Clr = Clr, typename _Uv = Vec2us>
	struct TClrVertex
	{
		_Ty pos;
		_Clr clr;
		_Uv uv;
	};

	template <typename _Ty, typename _Uv = Vec2us, typename _Nm = Vec3f>
	struct TNrmVertex
	{
		_Ty pos;
		_Uv uv;
		_Nm nrm;
	};

	using Vrtx2 = TVertex<Vec2f>;
	using Vrtx3 = TVertex<Vec3f>;

	using VrtxC2 = TClrVertex<Vec2f>;
	using VrtxC3 = TClrVertex<Vec3f>;

	using Vrtx8C2 = TClrVertex<Vec2f, Clr8>;
	using Vrtx8C3 = TClrVertex<Vec3f, Clr8>;

	using VrtxN3 = TNrmVertex<Vec3f, Clr8, Vec3f>;

	using Vrtx16N3 = TNrmVertex<Vec3f, Clr8, Vec3s>;

}
