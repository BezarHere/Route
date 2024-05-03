#pragma once
#include "Scene.h"
#include "Window.h"
#include "RenderCommandQueue.h"
#include "GraphicsResourceFactory.h"


namespace route
{
	class Application;



	class RenderCommandQueue
	{
	public:
		using command_boxed = rcq::command_boxed;


		vector<command_boxed> commands;
	};

	typedef uint64_t RendererLock;
	typedef void *RendererContext;

	class Renderer
	{
		friend GphxResFac;
	public:
		Renderer( Window &window );
		~Renderer();

		void render( const Application &app );

		inline GphxResFac &get_factory() {
			return m_factory;
		}

		inline const GphxResFac &get_factory() const {
			return m_factory;
		}

	private:
		Renderer( const Renderer & ) = delete;
		Renderer &operator =( const Renderer & ) = delete;

		static void bind_shader( const Shader &shader );
		static void clear_bound_shader();

		static void _bind_buffer( StorageBufferID buffer );

		void _do_command( const RenderCommandQueue::command_boxed &command );

	private:
		class State;
		RendererLock m_lock;
		Window &m_window;
		State *m_state;
		RendererContext m_context;
		RenderCommandQueue m_command_queue;

		// the factory should always be the last member field, so it can be initialized after all other fields
		GphxResFac m_factory;
	};

}
