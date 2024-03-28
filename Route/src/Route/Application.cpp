#include "pch.h"
#include "../pch.h"
#include "Application.h"
#include <thread>
#include <mutex>
#include "ResourceServer.h"
#include "Image.h"
#include "Texture.h"


namespace route
{
	struct Application::RSBC
	{
	public:

		template <typename... _Tys>
		static inline void execute( bool activate ) {
			return activate ? open<_Tys...>() : close<_Tys...>();
		}

		template <typename _Ty>
		static inline void open() {
			ResourceServer<_Ty>::open();
		}

		template <typename _Ty>
		static inline void close() {
			ResourceServer<_Ty>::close();
		}

		// two single parameters for overloading (packed args are optional which is not overloaded)
		template <typename _Ty, typename _Ey, typename... _Tys>
		static inline void open() {
			ResourceServer<_Ty>::open();
			ResourceServer<_Ey>::open();
			if constexpr (sizeof...(_Tys))
				RSBC::open<_Tys...>();
		}

		// two single parameters for overloading (packed args are optional which is not overloaded)
		template <typename _Ty, typename _Ey, typename... _Tys>
		static inline void close() {
			ResourceServer<_Ty>::close();
			ResourceServer<_Ey>::close();
			if constexpr (sizeof...(_Tys))
				RSBC::close<_Tys...>();
		}
	};

	struct Application::threading
	{
		std::thread update_thread;
		std::mutex loc;
		std::atomic_bool x;
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

	void Application::_toggle_resource_servers( bool new_state ) {
		RSBC::open<Image, Texture>( );
	}

}
