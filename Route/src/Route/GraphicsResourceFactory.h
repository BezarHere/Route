#pragma once
#include "ResourceServer.h"
#include "StorageBuffer.h"

namespace route
{
	using DeviceID = int64_t;
	typedef class GraphicsResourceFactory GphxResFac;

	class GraphicsResourceFactory
	{
	public:
		RID create_buffer( StorageBufType type );
		RID create_texture( StorageBufType type );

	private:
		DeviceID m_device;
	};


}
