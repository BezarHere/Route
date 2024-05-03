#pragma once
#include "ResourceServer.h"
#include "StorageBuffer.h"
#include "Texture.h"

namespace route
{
	using DeviceID = int64_t;
	typedef class GraphicsResourceFactory GphxResFac;

	class Renderer;

	class GraphicsResourceFactory
	{
		friend Renderer;
	public:
		RID create_buffer( StorageBufType type );
		RID create_texture( const TextureInfo &info );

		inline Error error() const {
			return m_error;
		}

		bool is_locked() const;

	private:
		GraphicsResourceFactory( Renderer &renderer );

		void _create_begin();
		void _register( const GraphicsResource &res, RID rid );

	private:
		Renderer &m_renderer;
		DeviceID m_device;
		Error m_error;
		vector<RID> m_resources;
	};


}
