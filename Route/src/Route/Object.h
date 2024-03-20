#pragma once
#include "Transform.h"
#include "Component.h"

namespace route
{
	class NodeTree;
	class BaseObject;

	typedef string object_path_t;
	typedef string object_name_t;

	enum class ObjectType
	{
		Object2D,
		Object3D,
	};



	class BaseObject
	{
		friend NodeTree;
	public:
		static constexpr size_t object_max_size = 380;



	protected:
		const ObjectType m_type;
		object_name_t m_name;
		index_t m_parent;
		vector<index_t> m_children;
		vector<component_boxed> m_children;
		bool m_enabled;
	};

	using object = BaseObject;

	template <typename _Trn>
	class TObject : public BaseObject
	{
	public:
		using this_type = TObject<_Trn>;
		using transform_type = _Trn;
		using vector_type = typename transform_type::vector_type;

		struct GlobalCache
		{
			transform_type transform;
			bool enabled;
		};

	private:
		transform_type m_transform;
		GlobalCache m_global;
	};

	using object_2d = TObject<Transform2D>;
	using object_3d = TObject<Transform3D>;

	using object_boxed = boxed<BaseObject, placement_block<BaseObject::object_max_size>>;

}
