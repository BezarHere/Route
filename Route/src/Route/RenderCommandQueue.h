#pragma once
#include "ArrayView.h"
#include "Primitives.h"
#include "ResourceServer.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"
#include "Vertex.h"

namespace route
{
#pragma region(enums)
	enum class PrimitiveTopology
	{
		Triangles,
		TriangleStrips,
		TriangleFans,
		Line,
		LineStrips,
		Points,
		Patches,
	};

	enum class RendererAttribute
	{
		DepthTest,
		// TODO: a lot of stuff (blending, scissors, ...)
	};

	enum class IndexType
	{
		Invalid = -1,
		UInt8,
		UInt16,
		UInt32,
	};

	enum class CompareOperator
	{
		Never,
		Less,
		Equal,
		LessOrEqual,
		Greater,
		NotEqual,
		GreaterOrEqual,
		Always
	};

	enum class CullMode
	{
		None,
		Front,
		Back,
		FrontAndBack
	};

	enum class FaceWinding
	{
		CounterClockwise,
		Clockwise,
	};
#pragma endregion

#pragma region(commands)

	namespace rcq
	{
		enum class CommandType
		{
			Draw,
			DrawVertices,
			BindVertexSource,
			Transform2D,
			Transform3D,
			Texture,
			NormalMap,
			Metrial,
			Shader,
			Primitive2D,
			Primitive3D,
			Polygon,
			Mesh,
		};

		enum class Filter
		{
			Nearest,
			Linear,
		};

		enum class EdgeSampling
		{
			Repeate,
			MirrorRepeate,
			ClampToEdge,
			ClampToBoarder
		};

		struct Command
		{
			inline constexpr Command( CommandType p_type )
				: type{ p_type } {

			}

			const CommandType type;
		};

		template <typename _Ty, CommandType _Type>
		struct TCommandSWrapper : public Command
		{
			using wrapped_type = _Ty;
			inline TCommandSWrapper()

				: Command{ _Type }, value{} {
			}
			inline TCommandSWrapper( const wrapped_type &p_value )
				: Command{ _Type }, value{ p_value } {
			}

			wrapped_type value;
		};

		template <typename _Ty, CommandType _Type>
		struct TCommandSSetResource : public Command
		{
			using resource_type = _Ty;

			inline TCommandSSetResource()
				: Command{ _Type }, rid{ RIDInvalid } {
			}

			inline TCommandSSetResource( RID p_rid )
				: Command{ _Type }, rid{ p_rid } {
			}

			inline resource_type &resource() {
				return ResourceServer<resource_type>::get_resource( rid );
			}

			inline const resource_type &resource() const {
				return ResourceServer<resource_type>::get_resource( rid );
			}

			inline bool can_invalidate() const {
				return ResourceServer<resource_type>::get_resource( rid ).change_counter() != change_counter;
			}

			RID rid;
			// for cache invalidation
			// if the resource has the same change counter, then it hasn't change
			refc_t change_counter;
		};

		// vertex buffer state is cleared after drawing
		struct CommandDrawVertices : public Command
		{

			inline CommandDrawVertices() : Command{ CommandType::DrawVertices } {
			}

			inline CommandDrawVertices( index_t p_offset, size_t p_count, PrimitiveTopology p_meshing_type )
				: Command{ CommandType::DrawVertices }, offset{ p_offset }, count{ p_count }, meshing_type{ p_meshing_type } {
			}


			// each vertex size is defined by the vertex input descriptor, see "VertexInputState"
			index_t offset = 0;
			size_t count = 0;
			PrimitiveTopology meshing_type = PrimitiveTopology::TriangleStrips;
		};

		struct CommandBindVertexSource : public Command
		{
			inline CommandBindVertexSource() : Command{ CommandType::BindVertexSource } {
			}

			inline CommandBindVertexSource( RID p_vertex_buffer, RID p_index_buffer, const Blob<const VertexInputState> &p_input_desc )
				: Command{ CommandType::BindVertexSource },
				vertex_buffer{ p_vertex_buffer }, index_buffer{ p_index_buffer }, input_desc{ p_input_desc } {
			}

			RID vertex_buffer;
			RID index_buffer;
			Blob<const VertexInputState> input_desc;
		};


		using CommandXForm2D = TCommandSWrapper<Transform2D, CommandType::Transform2D>;
		using CommandXForm3D = TCommandSWrapper<Transform3D, CommandType::Transform3D>;

		using CommandPrimitive2D = TCommandSWrapper<boxed_primitive_2d, CommandType::Primitive2D>;
		using CommandPrimitive3D = TCommandSWrapper<boxed_primitive_3d, CommandType::Primitive3D>;

		using CommandSetTexture = TCommandSWrapper<Texture, CommandType::Texture>;
		using CommandSetMaterial = TCommandSWrapper<Material, CommandType::Metrial>;
		using CommandSetShader = TCommandSWrapper<Shader, CommandType::Shader>;

		// TODO: the type shifter
		using command_boxed = boxed<Command, 128>;
	}

#pragma endregion
}
