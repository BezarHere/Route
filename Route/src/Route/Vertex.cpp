#include "pch.h"
#include "../pch.h"
#include "Vertex.h"

static inline VInputDescID create_mesh_desc() {
	VInputDescID name = 0;
	glGenVertexArrays( 1, &name );
	return name;
}

static inline constexpr size_t sizeof_attribute_type( const VertexInputType type ) {
	switch (type)
	{
	case VertexInputType::Byte:
		return sizeof( int8_t );
	case VertexInputType::UnsignedByte:
		return sizeof( uint8_t );
	case VertexInputType::Short:
		return sizeof( int16_t );
	case VertexInputType::UnsignedShort:
		return sizeof( uint16_t );
	case VertexInputType::Int:
		return sizeof( int32_t );
	case VertexInputType::UnsignedInt:
		return sizeof( uint32_t );
	case VertexInputType::HalfFloat:
		return sizeof( uint16_t );
	case VertexInputType::Float:
		return sizeof( float );
	case VertexInputType::Double:
		return sizeof( double );
	default:
		return sizeof( float );
	}
}

class VPDSetupRegister
{
public:

	static inline void set_enabled_attrs( const size_t num ) {
		if (num > VertexInputDesc::MaxVertexAttributes)
		{
			s_enabled_attrs = VertexInputDesc::MaxVertexAttributes;
			return;
		}
		s_enabled_attrs = num;
	}

	static inline size_t get_enabled_attrs() {
		return s_enabled_attrs;
	}

private:
	static size_t s_enabled_attrs;
};

size_t VPDSetupRegister::s_enabled_attrs = 0;

namespace route
{

	VertexInputDesc::VertexInputDesc()
		: m_id{ create_mesh_desc() }, m_container{}, m_offset{ 0 }, m_stride{ 0 } {
	}

	VertexInputDesc::~VertexInputDesc() {
		if (m_id)
		{
			glDeleteVertexArrays( 1, &m_id );
		}
	}

	VertexInputDesc::VertexInputDesc( const VertexInputDesc &copy )
		: m_id{ create_mesh_desc() }, m_container{ copy.m_container }, m_offset{ copy.m_offset }, m_stride{ copy.m_stride } {
	}

	VertexInputDesc::VertexInputDesc( VertexInputDesc &&move ) noexcept
		: m_id{ move.m_id }, m_container{ move.m_container }, m_offset{ move.m_offset }, m_stride{ move.m_stride } {
		move.m_id = 0;
	}

	VertexInputDesc &VertexInputDesc::operator=( const VertexInputDesc &copy ) {
		if (!m_id)
		{
			m_id = create_mesh_desc();
		}

		m_container = copy.m_container;
		m_offset = copy.m_offset;
		m_stride = copy.m_stride;
		return *this;
	}

	VertexInputDesc &VertexInputDesc::operator=( VertexInputDesc &&move ) noexcept {
		if (m_id)
		{
			glDeleteVertexArrays( 1, &m_id );
		}

		m_id = move.m_id;

		m_container = move.m_container;
		m_offset = move.m_offset;
		m_stride = move.m_stride;
		return *this;
	}

	void VertexInputDesc::bind() const {
		glBindVertexArray( m_id );
	}

	void VertexInputDesc::setup() const {
		const size_t count = std::min( m_container.size(), MaxVertexAttributes );
		VPDSetupRegister::set_enabled_attrs( count );

		size_t offset = m_offset;
		const size_t stride = m_stride ? m_stride : get_vertex_size();

		for (size_t i = 0; i < count; i++)
		{
			glEnableVertexAttribArray( static_cast<GLuint>(i) );
			glVertexAttribPointer(
				static_cast<GLuint>(i),
				static_cast<GLint>(m_container[ i ].size),
				static_cast<GLenum>(m_container[ i ].type) & (NormalizedVInputTypeBit - 1),
				static_cast<GLboolean>(static_cast<GLenum>(m_container[ i ].type) & NormalizedVInputTypeBit),
				static_cast<GLsizei>(stride),
				reinterpret_cast<const void *>(offset)
			);
			offset += static_cast<size_t>(m_container[ i ].size) * sizeof_attribute_type( m_container[ i ].type );
		}
	}

	VInputDescID VertexInputDesc::get_bound() {
		GLint value = 0;
		glGetIntegerv( GL_VERTEX_ARRAY_BINDING, &value );
		return value;
	}

	void VertexInputDesc::clear_bound() {
		glBindVertexArray( 0 );
	}

	void VertexInputDesc::clear_setup() {
		for (size_t i = 0; i < VPDSetupRegister::get_enabled_attrs(); i++)
		{
			glDisableVertexAttribArray( static_cast<GLuint>(i) );
		}
	}

	void VertexInputDesc::set_stride( uint32_t stride ) {
		m_stride = stride;
	}

	void VertexInputDesc::set_offset( uint32_t offset ) {
		m_offset = offset;
	}

	size_t VertexInputDesc::get_vertex_size() const {
		size_t attrs_size = 0;
		for (const auto &attr : m_container)
		{
			attrs_size += static_cast<size_t>(attr.size) * sizeof_attribute_type( attr.type );
		}
		return attrs_size;
	}

	void VertexInputDesc::_validate_attributes() {
		if (m_container.size() > MaxVertexAttributes)
		{
			m_container.resize( MaxVertexAttributes );
		}
	}

}
