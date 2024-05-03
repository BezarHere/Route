#pragma once
#include "Resource.h"

namespace route
{
	enum class StorageBufType
	{
		Invalid = -1,
		Vertex,
		Index,
		Texture,
		UniformBlock,
		ShaderStorage,
		TransformFeedback,
		DrawIndirect,
		CopySource,
		CopyDestination,
		QueryResult,
	};

	typedef vpid_t StorageBufferID;

	class StorageBuffer : public GraphicsResource
	{
		friend GraphicsResourceFactory;
	public:
		StorageBuffer( const StorageBuffer & );
		StorageBuffer( StorageBuffer && ) noexcept;
		StorageBuffer &operator=( const StorageBuffer & );
		StorageBuffer &operator=( StorageBuffer && ) noexcept;

		static StorageBufType read_type( StorageBufferID bid );

		FORCE_INLINE StorageBufferID id() const {
			return m_id;
		}

		FORCE_INLINE StorageBufType type() const {
			return read_type( m_id );
		}

		size_t size() const;
		void bind();

		errno_t update( const void *data, size_t size, index_t offset = 0 );

		static StorageBufferID get_bound( uint32_t type );
		static void clear_bound( uint32_t type );
		static void send_data( StorageBufferID id, const void *data, ptrdiff_t size, ptrdiff_t offset );



		
		

	private:
		// can lead to segfaults if not used carefully
		inline StorageBuffer( const StorageBufferID id ) : m_id{ id } {
		}

	private:
		StorageBufferID m_id;
	};

}
