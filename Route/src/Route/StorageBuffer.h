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

		_Max,
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

		FORCE_INLINE StorageBufferID get_id() const {
			return m_id;
		}

		FORCE_INLINE StorageBufType get_type() const {
			return m_type;
		}

		FORCE_INLINE size_t get_size() const {
			return m_size;
		}

	private:
		// can lead to segfaults if not used carefully
		inline StorageBuffer( const StorageBufferID id ) : m_id{ id } {
		}

	private:
		StorageBufferID m_id;
		StorageBufType m_type;
		size_t m_size;
	};

}
