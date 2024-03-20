#pragma once
#include "Object.h"


namespace route
{
	class NodeTree
	{
	public:

		inline object &get_object( const index_t index ) {
			return *m_objects[ index ];
		}

		inline const object &get_object( const index_t index ) const {
			return *m_objects[ index ];
		}

		template <typename _Pred>
		inline index_t find( const _Pred &predicate ) {
			for (size_t i = 0; i < m_objects.size(); i++)
			{
				if (predicate( *m_objects[ i ] ))
				{
					return i;
				}
			}
			return npos;
		}

		template <typename _Pred>
		inline const index_t find( const _Pred &predicate ) const {
			for (size_t i = 0; i < m_objects.size(); i++)
			{
				if (predicate( *m_objects[ i ] ))
				{
					return i;
				}
			}
			return npos;
		}

		inline void validate();

		inline void rebuild();

	private:

		inline void add_node_order( const index_t index ) {
			const auto &object = m_objects[ index ];

			if (!object->m_children.empty())
			{
				const size_t children_count = object->m_children.size();
				for (size_t i = children_count - 1; i < children_count; i--)
				{
					add_node_order( object->m_children[ i ] );
				}
			}

			m_order.push_back( index );
		}

	private:
		vector<object_boxed> m_objects;
		vector<index_t> m_roots;
		vector<index_t> m_order;
	};

	inline void NodeTree::validate() {
		const size_t objects_count = m_objects.size();
		for (index_t index = 0; index < objects_count; index++)
		{
			const auto &obj = m_objects[ index ];
			if (obj->m_parent != npos)
			{
				if (obj->m_parent >= m_objects.size())
				{
					//! OUT OF RANGE PARENT INDEX
				}

			}

			for (index_t child : obj->m_children)
			{
				if (child >= m_objects.size())
				{
					//! INVALID CHILD INDEX
				}

				if (m_objects[ child ]->m_parent != index)
				{
					// EASLY FIXABLE, BUT SHOULD IT BE FIXED?
					// WHAT IF THERE IS ANOTHER OBJECT EXPECTING THIS TO BE HIS CHILD??
					m_objects[ child ]->m_parent = index;
				}
			}
		}
	}

	inline void NodeTree::rebuild() {
		m_roots.clear();
		m_order.clear();

		for (index_t i = 0; i < m_objects.size(); i++)
		{
			if (m_objects[ i ]->m_parent == npos)
			{
				m_roots.emplace_back( i );
			}
		}

		const size_t roots_count = m_roots.size();
		for (size_t i = roots_count - 1; i < roots_count; i--)
		{
			add_node_order( m_roots[ i ] );
		}

	}

}

