#include "pch.h"
#include "Component.h"
#include "Object.h"

template route::TSpaceComponent<route::traits::Impl2D>;
template route::TSpaceComponent<route::traits::Impl3D>;

namespace route
{

	template<typename _Tx>
	inline bool TSpaceComponent<_Tx>::has_same_space( const Object &obj ) {
		if (obj.type == ObjectType::Object2D)
			return std::is_same_v<transform_type, Transform2D>;
		return std::is_same_v<transform_type, Transform3D>;
	}

}
