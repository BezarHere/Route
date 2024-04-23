#include "pch.h"
#include "Shader.h"
#include "../pch.h"
#include "Logger.h"

typedef intptr_t ShaderModuleID;



static inline constexpr ShaderModuleID to_native_type( ShaderModuleType type ) {
	constexpr ShaderModuleID Map[] = { GL_VERTEX_SHADER, GL_FRAGMENT_SHADER, GL_GEOMETRY_SHADER };
	return Map[ (int)type ];
}

static inline ShaderModuleID create_shader( ShaderModuleType type, const char *src ) {
	ShaderModuleID id = glCreateShader( to_native_type( type ) );

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

	return id;
}

static inline ShaderID compile_program( ShaderModuleID vertex, ShaderModuleID fragment, ShaderModuleID geometry ) {
	ShaderID prog = glCreateProgram();
	GL_CALL( glAttachShader( prog, vertex ) );
	GL_CALL( glAttachShader( prog, fragment ) );

	if (geometry)
	{
		GL_CALL( glAttachShader( prog, geometry ) );
	}

	GL_CALL( glLinkProgram( prog ) );

	GLint program_linked;
	glGetProgramiv( prog, GL_LINK_STATUS, &program_linked );
	if (!program_linked)
	{
		GLsizei len = 0;
		GLchar msg[ 1024 ];
		glGetProgramInfoLog( prog, 1024, &len, msg );
		Logger::write( string( "GL::Program error: " ) + msg, LogLevel::Error );
		glDeleteProgram( prog );
		return 0;
	}
	return prog;
}

static inline void destroy_program( ShaderID prog ) {
#ifdef GAPI_GL
	GL_CALL( glDeleteProgram( prog ) );
#endif
}

static inline void destroy_shader_module( ShaderModuleID module ) {
#ifdef GAPI_GL
	GL_CALL( glDeleteShader( module ) );
#endif
}

namespace route
{
	static const string EmptyStr = "";

	Shader::Shader() {
		// TODO: default shader
	}

	Shader::Shader( const string &vertex, const string &fragment ) : Shader( vertex, fragment, EmptyStr ) {
	}

	Shader::Shader( const string &vertex, const string &fragment, const string &geometry )
		: m_id{}, m_source{ vertex, fragment, geometry } {
		ShaderModuleID shader_vx =
			create_shader( ShaderModuleType::Vertex, vertex.c_str() );

		ShaderModuleID shader_fg =
			create_shader( ShaderModuleType::Fragment, fragment.c_str() );

		ShaderModuleID shader_ge =
			geometry.empty() ? NULL : create_shader( ShaderModuleType::Geometry, geometry.c_str() );

		m_id = compile_program( shader_vx, shader_fg, shader_ge );

		destroy_shader_module( shader_vx );
		destroy_shader_module( shader_fg );
		destroy_shader_module( shader_ge );
	}

	Shader::Shader( std::nullptr_t ) : m_id{}, m_source{} {
	}

	Shader::Shader( const Shader &copy ) : Shader( copy.m_source[ 0 ], copy.m_source[ 1 ], copy.m_source[ 2 ] ) {
	}

	Shader::Shader( Shader &&move ) noexcept
		: m_id{ move.m_id }, m_source{ move.m_source } {
		move.m_id = NULL;
	}

	Shader &Shader::operator=( const Shader &copy ) {
		return *this = Shader( copy.m_source[ 0 ], copy.m_source[ 1 ], copy.m_source[ 2 ] );
	}

	Shader &Shader::operator=( Shader &&move ) noexcept {
		destroy_program( m_id );
		m_id = move.m_id;
		move.m_id = NULL;

		m_source = move.m_source;
		return *this;
	}

}
