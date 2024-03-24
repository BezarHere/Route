#pragma once
#include "Bases.h"
#include "NodeTree.h"
#include "Window.h"
#include "Renderer.h"

namespace route
{
	class Engine
	{
	public:

		inline Engine( Window &window, Renderer &renderer )
			: m_window{ window }, m_renderer{ renderer } {
		}

		inline NodeTree &node_tree() noexcept {
			return m_tree;
		}

		inline const NodeTree &node_tree() const noexcept {
			return m_tree;
		}

	private:
		NodeTree m_tree;
		Renderer &m_renderer;
		Window &m_window;
	};
}
