#pragma once
#include "NodeTree.h"


namespace route
{
	class Renderer
	{
	public:

		void render( const NodeTree &tree );

	private:

	private:
		class State;
		std::unique_ptr<State> m_state;
	};

}
