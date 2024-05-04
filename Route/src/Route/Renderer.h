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

	struct RendererState
	{
		array<StorageBufferID, (int)StorageBufType::_Max> Buffers{};
		array<ShaderID, 3> Shaders{};
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

		void bind_buffer( const StorageBuffer &buffer );
		void bind_shader( const Shader &shader );

		inline StorageBufferID get_bound_buffer( StorageBufType type ) const {
			return m_state.Buffers[ static_cast<size_t>(type) ];
		}

		inline StorageBufferID get_bound_shader( ShaderType type ) const {
			return m_state.Shaders[ static_cast<size_t>(type) ];
		}

	private:
		Renderer( const Renderer & ) = delete;
		Renderer &operator =( const Renderer & ) = delete;

		void _do_command( const RenderCommandQueue::command_boxed &command );

	private:
		class APIState;
		RendererLock m_lock;
		Window &m_window;
		RendererState m_state;

		RenderCommandQueue m_command_queue;

		RendererContext m_context;
		APIState *m_api_state;
		// the factory should always be the last member field, so it can be initialized after all other fields
		GphxResFac m_factory;
	};

}
