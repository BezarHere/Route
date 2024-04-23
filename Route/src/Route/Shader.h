#pragma once
#include "Resource.h"

namespace route
{
	typedef vpid_t ShaderID;

	enum class ShaderModuleType
	{
		Vertex,
		Fragment,
		Geometry
	};

	class Shader : public GraphicsResource
	{
		friend Renderer;
	public:
		// default shader
		Shader();

		Shader( const string &vertex, const string &fragment );
		Shader( const string &vertex, const string &fragment, const string &geometry );

		// an invalid shader
		Shader( std::nullptr_t );

		Shader( const Shader &copy );
		Shader( Shader &&move ) noexcept;

		Shader &operator=( const Shader &copy );
		Shader &operator=( Shader &&move ) noexcept;

		inline bool is_valid() const {
			return m_id != 0;
		}

		inline operator bool() const {
			return m_id != 0;
		}


		inline bool operator!() const {
			return m_id == 0;
		}

		const string &get_source( ShaderModuleType module_type ) const;

	private:
		ShaderID m_id;
		array<string, 3> m_source;
	};

}
