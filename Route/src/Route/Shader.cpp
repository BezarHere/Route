#include "pch.h"
#include "Shader.h"
#include "../pch.h"
#include "Logger.h"


// using a semi-bad return type for vulkan (void ptr) & GL (name-id int)
static inline constexpr ShaderID to_native_type( ShaderType type ) {
#ifdef GAPI_GL
	constexpr GLuint Map[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
	return reinterpret_cast<ShaderID>(Map[ (int)type ]);
#endif
}

static inline ShaderID create_shader( ShaderType type, const char *src ) {
	GLuint id = glCreateShader( reinterpret_cast<GLuint>(to_native_type( type )) );
	
	// if the id is invalid
	if (id == 0)
	{
		Logger::write( format_join("Failed to create shader: ", glGetLastError()));
		return 0;
	}

	char header[ 32 ]{};
	sprintf_s( header, "#version %u\n", OpenGL::ContextVersion.major * 100 + OpenGL::ContextVersion.minor * 10 );

	// the header '#version ...' + the actual shader source
	const char *total_source[ 2 ] = {
		header, src
	};

	GL_CALL( glShaderSource( id, 2, total_source, nullptr ) );
	GL_CALL( glCompileShader( id ) );

	GLint compile_status;
	GL_CALL( glGetShaderiv( id, GL_COMPILE_STATUS, &compile_status ) );

	// compilation error
	if (!compile_status)
	{
		int length;
		glGetShaderiv( id, GL_INFO_LOG_LENGTH, &length );
		char *msg = new char[ length ];

		// load the log msg to 'msg'
		glGetShaderInfoLog( id, length, &length, msg );
		Logger::write( string( "GL::Shader error: " ) + msg, LogLevel::Error );
		delete[] msg;
		glDeleteShader( id );
		return 0;
	}

	return reinterpret_cast<ShaderID>(id);
}

static inline void destroy_shader( ShaderID module ) {
#ifdef GAPI_GL
	GL_CALL( glDeleteShader( (GLuint)module ) );
#endif
}

namespace route
{
	static const string EmptyStr = "";

	Shader::Shader() {
		// TODO: default shader
	}

	Shader::Shader( const char *source, ShaderType type ) : m_source{ source ? source : "" }, m_type{ type } {
		if (!source)
		{
			Logger::write( "Null source passed to shader", LogLevel::Error );
			m_id = ShaderID{};
			return;
		}

		m_id = create_shader( type, source );
	}

	Shader::Shader( std::nullptr_t ) : m_id{}, m_source{} {
	}

	Shader::Shader( const Shader &copy ) : Shader( copy.m_source.c_str(), copy.m_type ) {
	}

	Shader::Shader( Shader &&move ) noexcept
		: m_id{ move.m_id }, m_type{ move.m_type }, m_source{ move.m_source } {
		move.m_id = NULL;
	}

	Shader::~Shader() noexcept {
		destroy_shader( m_id );
	}

	Shader &Shader::operator=( const Shader &copy ) {
		return *this = Shader( copy.m_source.c_str(), copy.m_type);
	}

	Shader &Shader::operator=( Shader &&move ) noexcept {
		destroy_shader( m_id );
		m_id = move.m_id;
		m_type = move.m_type;
		m_source = move.m_source;

		move.m_id = NULL;
		return *this;
	}

	const string &Shader::get_source( ShaderType module_type ) const {
		return m_source[ (int)module_type ];
	}

}
