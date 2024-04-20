#pragma once
#include "Resource.h"

namespace route
{
	typedef unsigned ShaderID;

	class Shader : public GraphicsResource
	{
	public:

		

	private:
		Shader( const Shader & ) = delete;
		Shader &operator=( const Shader & ) = delete;
	private:
		ShaderID m_id;
	};

}
