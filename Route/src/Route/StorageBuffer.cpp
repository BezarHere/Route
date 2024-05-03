#include "pch.h"
#include "../pch.h"
#include "StorageBuffer.h"
#include "IMap.h"

/*
	OpenGL
	NOTE: not to be used with external OpenGL code, there is a lot impl-specific things

	The buffer id is composed as follows (while in opengl):
		0xFFFF'FFFF FFFF'FFFF     (uint64_t)
		 [  type  ] [ name (id) ]
*/

static FORCE_INLINE constexpr uint32_t gl_name( const StorageBufferID id ) {
	return id & 0xFFFF'FFFFULL; // first 32 bits
}

static FORCE_INLINE constexpr uint32_t gl_type( const StorageBufferID id ) {
	return id >> 32U; // last 32 bits
}

namespace route
{

	StorageBuffer::StorageBuffer( const StorageBuffer & ) {
	}

	StorageBuffer::StorageBuffer( StorageBuffer && ) noexcept {
	}

	StorageBuffer &StorageBuffer::operator=( const StorageBuffer & ) {
		return *this;
	}

	StorageBuffer &StorageBuffer::operator=( StorageBuffer && ) noexcept {
		return *this;
	}

	StorageBufType StorageBuffer::read_type( StorageBufferID bid ) {
#ifdef GAPI_GL
		switch (gl_type( bid ))
		{
		case GL_ARRAY_BUFFER:
			return StorageBufType::Vertex;
		case GL_ELEMENT_ARRAY_BUFFER:
			return StorageBufType::Index;
		case GL_TEXTURE_BUFFER:
			return StorageBufType::Texture;

		case GL_UNIFORM_BUFFER:
			return StorageBufType::UniformBlock;
		case GL_SHADER_STORAGE_BUFFER:
			return StorageBufType::ShaderStorage;

		case GL_TRANSFORM_FEEDBACK_BUFFER:
			return StorageBufType::TransformFeedback;
		case GL_DRAW_INDIRECT_BUFFER:
			return StorageBufType::DrawIndirect;

		case GL_COPY_READ_BUFFER:
			return StorageBufType::CopySource;
		case GL_COPY_WRITE_BUFFER:
			return StorageBufType::CopyDestination;

		case GL_QUERY_BUFFER:
			return StorageBufType::QueryResult;
		default:
			return StorageBufType::Invalid;
		}
#endif // GAPI_GL

	}

	size_t StorageBuffer::size() const {
		return OpenGL::query_int( GL_BUFFER_SIZE );
	}

#ifdef GAPI_GL
	void StorageBuffer::bind() {
		glBindBuffer( gl_type( m_id ), gl_name( m_id ) );
	}

	StorageBufferID StorageBuffer::get_bound( uint32_t type ) {
		return (static_cast<StorageBufferID>(type) << 32U) | OpenGL::query_int( type );
	}

	void StorageBuffer::clear_bound( uint32_t type ) {
		glBindBuffer( type, 0 );
	}
#endif

	errno_t StorageBuffer::update( const void *data, const size_t size, const index_t offset ) {
		send_data( gl_type( m_id ), data, size, offset );
		return errno_t();
	}

	void StorageBuffer::send_data( StorageBufferID id, const void *data, ptrdiff_t size, ptrdiff_t offset ) {
		
#ifdef GAPI_GL
		glNamedBufferSubData( gl_name( id ), offset, size, data );
#endif
	}

}
