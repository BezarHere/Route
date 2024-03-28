#pragma once
#include "Bases.h"

namespace route
{
	typedef unsigned ShaderID;

	class Shader
	{
	public:

		

	private:
		Shader( const Shader & ) = delete;
		Shader &operator=( const Shader & ) = delete;
	private:
		ShaderID m_id;
	};

}
