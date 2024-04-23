#pragma once
#include "Shader.h"

namespace route
{
	enum class ShaderFieldType
	{
		Float,
		Int,
		UInt,
		Bool,
		Float2,
		Float3,
		Float4,
		Int2,
		Int3,
		Int4,
		Mat2,
		Mat3,
		Mat4,
		Object,
	};
	enum class ShaderFieldPrec
	{
		Low,
		Med,
		High,
	};

	struct ShaderField
	{
		ShaderFieldType type;
		ShaderFieldPrec prec;
		string name;
		uint8_t count = 1;
	};

	struct ShaderIO
	{
		ShaderField field;
		int16_t location = -1;
		vector<ShaderField> nested; // <- for object types
	};

	struct ShaderUniform
	{
		ShaderField field;
		string default_value;
	};

	struct ShaderLayer
	{
		// #version {abs(version)} {'core' if 'version' > 0 else 'compatibility}
		uint16_t version;
		vector<ShaderIO> inputs;
		vector<ShaderIO> outputs;
		vector<ShaderUniform> uniforms;
		string code;
	};

	struct ShaderProgramCake
	{
		ShaderLayer vertex;
		ShaderLayer fragment;
		bool has_geometry = false;
		ShaderLayer geometry;
	};

	class ShaderFactory
	{


	};
}
