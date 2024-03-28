#pragma once
#include "Scene.h"
#include "Window.h"
#include "Primitives.h"


namespace route
{
	class Application;

	class RenderCommandQueue
	{
		enum class CommandType
		{
			Transform,
			Texture,
			NormalMap,
			Primitive,
			Polygon,
			Mesh,
			Rect,
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


		using CommandXForm2D = TCommandSWrapper<Transform2D, CommandType::Transform>;
		using CommandXForm3D = TCommandSWrapper<Transform3D, CommandType::Transform>;
		using CommandPrimitive2D = TCommandSWrapper<boxed_primitive_2d, CommandType::Primitive>;
		using CommandPrimitive3D = TCommandSWrapper<boxed_primitive_3d, CommandType::Primitive>;



	};

	class Renderer
	{
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
		std::unique_ptr<State> m_state;
		void *m_context;
	};

}
