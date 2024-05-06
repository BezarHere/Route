#pragma once
#include "Resource.h"

namespace route
{
	typedef vpid_t ShaderID;

	enum class ShaderType
	{
		Vertex,
		Fragment,
		Geometry
	};

	class Shader : public GraphicsResource
	{
		friend Renderer;
		friend GraphicsFactory;
	public:
		Shader( Shader &&move ) noexcept;

		~Shader() noexcept;

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

		inline ShaderID get_id() const {
			return m_id;
		}

		inline ShaderType get_type() const {
			return m_type;
		}


		inline const string &get_source() const {
			return m_source;
		}
	private:
		Shader( const char *source, ShaderType type, GraphicsFactory &factory );

	private:
		ShaderType m_type;
		ShaderID m_id;
		string m_source;
	};

}
