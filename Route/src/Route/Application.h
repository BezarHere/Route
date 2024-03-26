#pragma once
#include "Bases.h"
#include "Window.h"
#include "Renderer.h"
#include "Scene.h"

namespace route
{
	struct EngineHandle
	{
		virtual void operator()( Application & ) = 0;
	};

	struct EngineConfig
	{
		uint32_t max_frames_per_sec;
		bool vsync;
	};

	class Application
	{
	public:

		Application( Window &window, Renderer &renderer );
		~Application();

		inline Scene &scene() {
			return m_scene;
		}

		inline const Scene &scene() const {
			return m_scene;
		}

		inline void set_handle( std::unique_ptr<EngineHandle> &&handle ) {
			m_handle.swap( handle );
		}

		inline EngineHandle *get_handle() {
			return m_handle.get();
		}

		inline const EngineHandle *get_handle() const {
			return m_handle.get();
		}

		errno_t start();
		void stop();

	private:
		void _process();
		void _graphics();
		void _gen_render_map();

		Application( const Application & ) = delete;
		Application &operator=( const Application & ) = delete;

	private:
		struct threading;
		struct cache;
		bool m_started = false;

		threading *m_threading;
		cache *m_cache;

		Scene m_scene;
		Renderer &m_renderer;
		Window &m_window;

		std::unique_ptr<EngineHandle> m_handle;

		EngineConfig m_config;

		time_point_us m_last_frame;
	};
}
