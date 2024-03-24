#pragma once
#include "Transform.h"
#include "Component.h"

namespace route
{
	class NodeTree;
	class Object;

	using object_path = string;
	using object_name = string;
	typedef int16_t zindex_t;

	enum class ObjectType
	{
		Object2D,
		Object3D,
	};

	class Object
	{
		friend NodeTree;
	public:


		inline Object( const ObjectType type )
			: m_type{ type } {
		}

		~Object() {
		}

		inline void update( real_t deltatime );

		inline index_t add_component( std::unique_ptr<Component> comp ) {
			m_components.emplace_back( comp.release() );
			return m_components.size() - 1;
		}
	private:
		void update_2d( real_t deltatime );
		void update_3d( real_t deltatime );

	private:

		const ObjectType m_type;

		//object_name m_name = "";
		index_t m_parent = npos;
		vector<index_t> m_children{};
		vector<std::unique_ptr<Component>> m_components{};
		bool m_active = true;
		bool m_visible = true;
	};

	inline void Object::update( real_t deltatime ) {
		for (auto &comp : m_components)
		{
			comp->update( *this );
		}

		if (m_type == ObjectType::Object2D)
		{
			return update_2d( deltatime );
		}
		return update_3d( deltatime );
	}

	using object = Object;

}
