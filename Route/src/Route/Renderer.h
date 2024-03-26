#pragma once
#include "Scene.h"
#include "Window.h"


namespace route
{
	class Application;

	class Renderer
	{
	public:
		Renderer( Window &window );
		~Renderer();

		void render( const Application &app );

	private:
		Renderer( const Renderer & ) = delete;
		Renderer &operator =( const Renderer & ) = delete;
	private:
		class State;
		Window &m_window;
		std::unique_ptr<State> m_state;
		void *m_context;
	};

}
