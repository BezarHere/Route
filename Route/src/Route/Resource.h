#pragma once
#include "Bases.h"


namespace route
{
	using RID = uint64_t;
	_INLINE_VAR constexpr auto RIDInvalid = RID( -1 );

	// base class for data objects handled by their RID
	// Resource usually are allocated/deallocated by their ResourceServer<_Ty>
	// any resource of type 'Ty' lives in the 'ResourceServer<Ty>'
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
	// resource that are create or handled by a GraphicsResourceFactory
	class GraphicsResource : public Resource
	{
		friend GraphicsResourceFactory;
	public:
		inline GraphicsResourceFactory *factory() const {
			return m_factory;
		}

	protected:
		inline void set_factory( GraphicsResourceFactory *new_factory ) {
			m_factory = new_factory;
		}

	private:
		mutable GraphicsResourceFactory *m_factory;
	};
}
