#pragma once
#include "Bases.h"


namespace route
{
	using RID = uint64_t;
	_INLINE_VAR constexpr auto RIDInvalid = RID( -1 );

	// base class for data objects handled by their RID
	// Resources are allocated/deallocated by their ResourceServer<_Ty>
	// any resource of type 'Ty' lives in the 'ResourceServer<Ty>'
	// every RID is unique between all resource servers (not just the resource server it belongs to)
	class Resource
	{
	public:
		inline refc_t change_counter() const {
			return m_change_counter;
		}

	protected:
		inline void inc_change_counter() {
			++m_change_counter;
		};
	private:
		refc_t m_change_counter;
	};

	class GraphicsResourceFactory;
	class Renderer;
	// resource that are create or handled by a GraphicsResourceFactory
	class GraphicsResource : public Resource
	{
		friend GraphicsResourceFactory;
		friend Renderer;
	public:
		using factory = GraphicsResourceFactory;

		inline factory *get_factory() const {
			return m_factory;
		}

	protected:
		inline GraphicsResource( factory *pfactory ) : m_factory{ pfactory } {
			RT_ASSERT_RELEASE( m_factory );
		}

		inline GraphicsResource() : m_factory{ nullptr } {
		}

		inline void set_factory( factory *new_factory ) {
			m_factory = new_factory;
		}

	private:
		mutable factory *m_factory;
	};

	// base class for all resources that can be load/saved to a file in the disk
	class IOResource : public Resource
	{
	public:

		inline virtual errno_t save( const string &path ) const {

		};
		inline virtual errno_t load( const string &path ) = 0;

	protected:
		string m_filepath;
	};

	// base class for all resources that can be load/saved to a file in the disk
	class UserResource : public Resource
	{
	public:

	protected:
		void *m_data;
	};
}
