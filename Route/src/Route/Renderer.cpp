#include "pch.h"
#include "../pch.h"
#include "Logger.h"
#include "Renderer.h"
#include "StorageBuffer.h"




namespace route
{
	class Renderer::State
	{
	public:

		State() {
			auto vertex_shader =
				"layout (location = 0) in vec4 pos; out vec3 clr;"
				"void main() { gl_Position = pos; clr = vec3((pos.xy + vec2(1)) / 2.0, 0.5); }"
				;
			auto frag_shader =
				"in vec3 clr; layout (location = 0) out vec4 Color;"
				"void main() { Color = vec4(clr, 1.0); }"
				;
			shader = Shader( vertex_shader, frag_shader );

		}

		~State() {
		}

		Shader shader;
		Vec2u last_window_size;
	};

	Renderer::Renderer( Window &window ) : m_window{ window } {
		// TODO: start OpenGL
		m_context = OpenGL::create_context( static_cast<LPSDLWindow>(m_window.m_handle) );
		if (!m_context)
		{
			Logger::write( "Failed to create context for Renderer", LogLevel::Error );
			return;
		}

		m_state = new State{};
	}

	Renderer::~Renderer() {
		const auto current_context = OpenGL::get_context();
		const auto current_window = OpenGL::get_context_window();

		OpenGL::set_context( m_window.m_handle, m_context );
		delete m_state;
		OpenGL::delete_context( m_context );

		OpenGL::set_context( current_window, current_context );
	}

	void Renderer::render( const Application &app ) {
		SDL_FNCHECK_V( OpenGL::set_context( m_window.m_handle, m_context ), return );

		constexpr Vec2f verts[]
		{
			{ 0.0f, 0.5f },
			{ 0.0f, 1.0f },
			{ 1.0f, 1.0f },
			{ 0.0f, 0.0f },
			{ -1.0f, 0.0f },
			{ -1.0f, -1.0f },
		};

		const auto wnd_sz = m_window.size();
		if (wnd_sz != m_state->last_window_size)
			glViewport( -32, -32, wnd_sz.x, wnd_sz.y );

		m_state->last_window_size = wnd_sz;

		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		GLuint vbo{ 0 };
		GLuint vao{ 0 };

		GL_CALL( glGenVertexArrays( 1, &vao ) );
		GL_CALL( glGenBuffers( 1, &vbo ) );
		GL_CALL( glBindVertexArray( vao ) );
		GL_CALL( glBindBuffer( GL_ARRAY_BUFFER, vbo ) );
		GL_CALL( glBufferData( GL_ARRAY_BUFFER, sizeof( Vec2f ) * std::size( verts ), verts, GL_STATIC_DRAW ) );

		GL_CALL( glEnableVertexAttribArray( 0 ) );
		GL_CALL( glVertexAttribPointer( 0, 2, GL_FLOAT, GL_FALSE, sizeof( Vec2f ), nullptr ) );

		bind_shader( m_state->shader );
		GL_CALL( glBindVertexArray( vao ) );
		GL_CALL( glBindBuffer( GL_ARRAY_BUFFER, vbo ) );

		GL_CALL( glDrawArrays( GL_TRIANGLES, 0, std::size( verts ) ) );

		GL_CALL( glBindBuffer( GL_ARRAY_BUFFER, 0 ) );
		GL_CALL( glBindVertexArray( 0 ) );
		clear_bound_shader();

		GL_CALL( glDeleteBuffers( 1, &vbo ) );
		GL_CALL( glDeleteVertexArrays( 1, &vao ) );

		SDL_GL_SwapWindow( static_cast<LPSDLWindow>(m_window.m_handle) );
	}

	void Renderer::bind_shader( const Shader &shader ) {
#ifdef GAPI_GL
		GL_CALL( glUseProgram( (GLuint)shader.m_id ) );
#endif
	}

	void Renderer::clear_bound_shader() {
#ifdef GAPI_GL
		glUseProgram( NULL );
#endif
	}


#ifdef GAPI_GL
	void Renderer::_do_command( const RenderCommandQueue::command_boxed &command ) {
		using rcq::CommandType;
		switch (command->type)
		{
		case CommandType::BindVertexSource:
			{
				const auto &c = command.get<rcq::CommandBindVertexSource>();
				ResourceServer<StorageBuffer>::get_resource( c.vertex_buffer );
			}
		default:
			break;
		}
	}
#endif
}
