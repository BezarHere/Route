#pragma once
#include "Scene.h"
#include "Window.h"
#include "RenderCommandQueue.h"


namespace route
{
	class Application;



	class RenderCommandQueue
	{
	public:
		using command_boxed = rcq::command_boxed;


		vector<command_boxed> commands;
	};

	class GraphicsResourceFactory;

	class Renderer
	{
		friend GraphicsResourceFactory;
	public:
		Renderer( Window &window );
		~Renderer();

		void render( const Application &app );

	private:
		Renderer( const Renderer & ) = delete;
		Renderer &operator =( const Renderer & ) = delete;

		static void bind_shader( const Shader &shader );
		static void clear_bound_shader();

		void _do_command( const RenderCommandQueue::command_boxed &command );

	private:
		class State;
		Window &m_window;
		State *m_state;
		void *m_context;
		RenderCommandQueue m_command_queue;
	};

}
