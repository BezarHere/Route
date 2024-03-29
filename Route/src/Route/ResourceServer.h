#pragma once
#include "Bases.h"

namespace route
{
	using RID = uint32_t;
	_INLINE_VAR constexpr auto RIDnpos = RID( -1 );

	class Application;
	class Renderer;

	template <typename _Ty>
	class ResourceServer
	{
		friend Application;
		friend Renderer;
	public:
		using resource_type = _Ty;
		using resource_name_char = char;
		static constexpr size_t ResourceNameMaxLn = 128;
		struct Internal;

		static resource_type &get_resource( RID rid );
		static RID add_resource( resource_type &&resource );
		static RID add_resource( const resource_type &resource );
		static void pop_resource( RID rid );

		/// @brief 
		/// @param rid the resource id
		/// @param name the name to be set, read until encountering a null-termination or reached the ResourceNameMaxLn
		static void set_resource_name( RID rid, const resource_name_char *name );

	private:

		/// @note increments the ref-counter if the server is already running
		static errno_t open();
		/// @returns true if the server closed, false otherwise (decremented ref counter)
		static bool close();
	private:
		static Internal *s_internal;
	};



}
