#pragma once
#include "Transform.h"
#include "Component.h"

namespace route
{
	class NodeTree;
	class BaseObject;

	using object_path = string;
	using object_name = string;
	typedef int16_t zindex_t;

	enum class ObjectType
	{
		Object2D,
		Object3D,
	};

	class BaseObject
	{
		friend NodeTree;
	public:
		static constexpr size_t object_max_size = 512;



	protected:
		const ObjectType m_type;
		object_name m_name;
		index_t m_parent;
		vector<index_t> m_children;
		vector<component_boxed> m_children;
		bool m_active;
		bool m_visible;
	};

	using object = BaseObject;

	class Object3D final : public BaseObject
	{
	public:
		using this_type = Object3D;
		using transform_type = Transform3D;
		using vector_type = typename transform_type::vector_type;

		struct GlobalCache
		{
			transform_type transform;
			bool active;
			bool visible;
		};

	private:
		transform_type m_transform;
		GlobalCache m_global;
	};

	class Object2D final : public BaseObject
	{
	public:
		using this_type = Object2D;
		using transform_type = Transform2D;
		using vector_type = typename transform_type::vector_type;

		struct GlobalCache
		{
			transform_type transform;
			zindex_t m_zindex;
			bool active;
			bool visible;
		};

	private:
		transform_type m_transform;
		zindex_t m_zindex;
		GlobalCache m_global;
	};

	using object_2d = Object2D;
	using object_3d = Object3D;

	static_assert(sizeof( object_2d ) <= BaseObject::object_max_size, "Object2D will be sliced inside a boxed object");
	static_assert(sizeof( object_3d ) <= BaseObject::object_max_size, "Object3D will be sliced inside a boxed object");

	using object_boxed = boxed<BaseObject, placement_block<BaseObject::object_max_size>>;

}
