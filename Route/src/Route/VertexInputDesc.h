#pragma once
#include "Resource.h"
#include "Vertex.h"

namespace route
{
	typedef uint64_t VertexInputDescID;
	class VertexInputDescriptor : public GraphicsResource
	{
	public:

		
	private:
		VertexInputDescID m_id;
		VertexInputState m_state;
	};
}
