#include "pch.h"
#include "../pch.h"
#include "Application.h"
#include <thread>
#include <mutex>
#include "ResourceServer.h"
#include "Image.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "Performance.h"


namespace route
{


	struct Application::threading
	{
		// for updating caches
		std::thread recahe_thread;
		std::thread update_thread;
	};

	struct Application::cache
	{
		static constexpr size_t render_map_init_capacity = 2024; // 16 kb on 64x

		inline cache() {
			render_map.reserve( render_map_init_capacity );
		}

		// indices of all objects who should be rendered (ordered)
		vector<index_t> render_map = {};
	};

	Application::Application( Window &window, Renderer &renderer )
		: m_window{ window }, m_renderer{ renderer }, m_threading{ nullptr }, m_cache{ new cache() } {
		_toggle_resource_servers( true );
	}

	Application::~Application() {
		if (m_threading)
			delete m_threading;
		if (m_cache)
			delete m_cache;
		_toggle_resource_servers( false );
	}

	errno_t Application::start() {
		if (m_threading)
		{
			delete m_threading;
			return EACCES;
		}

		m_threading = new threading();


		m_renderer.render( *this );

		m_threading->update_thread = std::thread( [ this ]() { _process(); } );
		_graphics();
		return 0;
	}

	void Application::stop() {
		_ASSERT( m_threading );

		if (m_threading->update_thread.joinable())
		{
			m_threading->update_thread.join();
		}

		delete m_threading;
		m_threading = nullptr;
	}

	void Application::_process() {
		std::cout << "Started _process\n";
		while (m_window)
		{
			std::this_thread::sleep_for( milliseconds( 20 ) );
		}

		std::cout << "Done _process\n";
	}

	void Application::_graphics() {
		std::cout << "Started _graphics\n";
		while (m_window)
		{
			m_window.poll();
			m_renderer.render( *this );
		}

		std::cout << "Done _graphics\n";
	}

	void Application::_gen_render_map() {
		// uhmmm, actually, it's a vector
		vector<index_t> &map = m_cache->render_map;
		map.clear();

		for (index_t obj_i : m_scene.order_indices())
		{
			const auto &obj = m_scene.get_object( obj_i );
			if (obj.is_visible_globally())
			{
				map.emplace_back( obj_i );
			}
		}
	}



}
