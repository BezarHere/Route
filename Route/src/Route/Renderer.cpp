#include "pch.h"
#include "../pch.h"
#include "Logger.h"
#include "Renderer.h"


namespace route
{
	class Renderer::State
	{

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

		constexpr GLfloat verts[]
		{
			0.0f, 1.0f, -1.0f, 0.0f, 1.0f, 0.0f
		};


		glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
		glClear( GL_COLOR_BUFFER_BIT );

		GLuint vbo{ 0 };
		GLuint vao{ 0 };

		glGenVertexArrays( 1, &vao );
		glGenBuffers( 1, &vbo );
		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glBufferData( GL_ARRAY_BUFFER, sizeof( GLfloat ) * 6, verts, GL_STATIC_DRAW );

		glEnableVertexAttribArray( 0 );
		glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 2 * sizeof( GLfloat ), nullptr );

		glBindBuffer( 0, vbo );
		glBindVertexArray( 0 );

		glBindVertexArray( vao );
		glBindBuffer( GL_ARRAY_BUFFER, vbo );
		glDrawArrays( GL_TRIANGLES, 0, 3 );
		glBindBuffer( GL_ARRAY_BUFFER, 0 );
		glBindVertexArray( 0 );

		glDeleteBuffers( 1, &vbo );
		glDeleteVertexArrays( 1, &vao );

		SDL_GL_SwapWindow( static_cast<LPSDLWindow>(m_window.m_handle) );
	}

}
