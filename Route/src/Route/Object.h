#pragma once
#include "Component.h"
#include "Traits.h"
#include "Memblock.h"

namespace route
{
	class Scene;
	class Object;
	class Component;

	using object_path = string;
	using object_name = string;
	typedef int16_t zindex_t;

	enum class ObjectType
	{
		Object2D,
		Object3D,
	};

	template <typename _Traits>
	struct TObjectState
	{
		using transform_type = typename _Traits::transform;
		bool visible = true;
		transform_type xform = {};
		typename _Traits::zindex zindex = {};
	};
	using ObjectState2D = TObjectState<traits::Impl2D>;
	using ObjectState3D = TObjectState<traits::Impl3D>;

	class Object
	{
		friend Scene;
	public:
		using State = Memblock<ObjectState2D, ObjectState3D>;


		inline Object( const ObjectType p_type )
			: type{ p_type }, m_state{}, m_global_cache{} {
		}

		Object( const Object &obj );
		Object &operator=( const Object &obj );

		~Object() {
		}

		inline void update( real_t deltatime );

		inline index_t add_component( std::unique_ptr<Component> &&comp ) {
			m_components.emplace_back( comp.release() );
			return m_components.size() - 1;
		}

		inline bool is_visible_globally() const {
			return m_state.get().visible && m_global_cache.get().visible;
		}

		inline bool is_visible() const {
			return m_state.get().visible;
		}

		const ObjectType type;
	private:
		void update_2d( real_t deltatime );
		void update_3d( real_t deltatime );

	private:

		//object_name m_name = "";
		index_t m_parent = npos;
		vector<index_t> m_children{};
		vector<std::unique_ptr<Component>> m_components{};
		bool m_active = true;

		State m_state;
		State m_global_cache;
	};



	using object = Object;

}
