#pragma once
#include "Scene.h"
#include "Window.h"
#include "ArrayView.h"
#include "Primitives.h"
#include "ResourceServer.h"
#include "Texture.h"
#include "Material.h"
#include "Shader.h"


namespace route
{
	class Application;


	// TODO: rename this, horrible name
	enum class Topology
	{
		Triangles,
		TriangleStrips,
		TriangleFans,
		Line,
		LineStrips,
		Points
	};

	class RenderCommandQueue
	{
		enum class CommandType
		{
			Draw,
			VertexBuffer,
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

		enum class CommandDrawType
		{
			VerticesRaw,
			VerticesIndexed
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
				: Command{ _Type }, rid{ RIDnpos } {
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
		struct CommandVertexBuffer : public Command
		{

			inline CommandVertexBuffer() : Command{ CommandType::VertexBuffer } {
			}

			inline CommandVertexBuffer( void *p_vertices, size_t p_count, Topology p_meshing_type )
				: Command{ CommandType::VertexBuffer }, vertices{ p_vertices }, count{ p_count }, meshing_type{ p_meshing_type } {
			}


			// each vertex size is defined by the vertex input descriptor, see "VertexInputDesc"
			void *vertices = nullptr;
			size_t count = 0;
			Topology meshing_type = Topology::Triangles;
		};


		using CommandDraw = TCommandSWrapper<CommandDrawType, CommandType::Draw>;

		using CommandXForm2D = TCommandSWrapper<Transform2D, CommandType::Transform2D>;
		using CommandXForm3D = TCommandSWrapper<Transform3D, CommandType::Transform3D>;

		using CommandPrimitive2D = TCommandSWrapper<boxed_primitive_2d, CommandType::Primitive2D>;
		using CommandPrimitive3D = TCommandSWrapper<boxed_primitive_3d, CommandType::Primitive3D>;

		using CommandSetTexture = TCommandSWrapper<Texture, CommandType::Texture>;
		using CommandSetMaterial = TCommandSWrapper<Material, CommandType::Metrial>;
		using CommandSetShader = TCommandSWrapper<Shader, CommandType::Shader>;


		using command_boxed = boxed<Command, 192>;
	};

	class RenderServer;

	class Renderer
	{
		friend RenderServer;
	public:
		Renderer( Window &window );
		~Renderer();

		void render( const Application &app );

	private:
		Renderer( const Renderer & ) = delete;
		Renderer &operator =( const Renderer & ) = delete;
	private:
		class State;
		Window &m_window;
		State *m_state;
		void *m_context;
	};

}
