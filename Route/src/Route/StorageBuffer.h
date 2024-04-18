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

	typedef uint64_t BufferID;

	class StorageBuffer : public Resource
	{
	public:
		StorageBuffer( const StorageBuffer & );
		StorageBuffer( StorageBuffer && ) noexcept;
		StorageBuffer &operator=( const StorageBuffer & );
		StorageBuffer &operator=( StorageBuffer && ) noexcept;

		static StorageBufType read_type( BufferID bid );

		FORCE_INLINE BufferID id() const {
			return m_id;
		}

		FORCE_INLINE StorageBufType type() const {
			return read_type( m_id );
		}

		size_t size() const;
		void bind();

		errno_t put( const void *data, size_t size, index_t offset = 0 );

		static BufferID get_bound( uint32_t type );
		static void clear_bound( uint32_t type );
		static void subdata( uint32_t type, const void *data, ptrdiff_t size, ptrdiff_t offset );



		
		

	private:
		// can lead to segfaults if not used carefully
		inline StorageBuffer( const BufferID id ) : m_id{ id } {
		}

	private:
		BufferID m_id;
	};

}
