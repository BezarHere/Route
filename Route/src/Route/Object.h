#pragma once
#include "Transform.h"
#include "Component.h"

namespace route
{
	class Scene;
	class Object;

	using object_path = string;
	using object_name = string;
	typedef int16_t zindex_t;

	enum class ObjectType
	{
		Object2D,
		Object3D,
	};

	struct ObjectState
	{

		bool visible = true;
		// having the 2d & 3d transforms
		// cons: larger size
		// pros: less headache
		Transform2D _2d = {};
		Transform3D _3d = {};
		zindex_t zindex = {};
	};

	class Object
	{
		friend Scene;
	public:


		inline Object( const ObjectType p_type )
			: type{ p_type }, m_state{}, m_global_cache{} {
		}

		~Object() {
		}

		inline void update( real_t deltatime );

		inline index_t add_component( std::unique_ptr<Component> &&comp ) {
			m_components.emplace_back( comp.release() );
			return m_components.size() - 1;
		}

		inline bool is_visible_globally() const {
			return m_state.visible && m_global_cache.visible;
		}

		inline bool is_visible() const {
			return m_state.visible;
		}

		const ObjectType type;
	private:
		void update_2d( real_t deltatime );
		void update_3d( real_t deltatime );

	private:
		//object_name m_name = "";
		index_t m_parent = npos;
		vector<index_t> m_children{};
		vector<std::shared_ptr<Component>> m_components{};
		bool m_active = true;

		ObjectState m_state;
		ObjectState m_global_cache;
	};

	inline void Object::update( real_t deltatime ) {
		for (const auto &comp : m_components)
		{
			comp->update( *this );
		}

		if (type == ObjectType::Object2D)
		{
			return update_2d( deltatime );
		}
		return update_3d( deltatime );
	}

	using object = Object;

}
