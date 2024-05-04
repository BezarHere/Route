#include "pch.h"
#include "../pch.h"
#include "VertexInputState.h"

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
		if (num > VertexInputState::MaxVertexAttributes)
		{
			s_enabled_attrs = VertexInputState::MaxVertexAttributes;
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

	VertexInputState::VertexInputState()
		: m_id{ create_mesh_desc() }, m_container{}, m_offset{ 0 }, m_stride{ 0 } {
	}

	VertexInputState::~VertexInputState() {
		if (m_id)
		{
			glDeleteVertexArrays( 1, &m_id );
		}
	}

	VertexInputState::VertexInputState( const VertexInputState &copy )
		: m_id{ create_mesh_desc() }, m_container{ copy.m_container }, m_offset{ copy.m_offset }, m_stride{ copy.m_stride } {
	}

	VertexInputState::VertexInputState( VertexInputState &&move ) noexcept
		: m_id{ move.m_id }, m_container{ move.m_container }, m_offset{ move.m_offset }, m_stride{ move.m_stride } {
		move.m_id = 0;
	}

	VertexInputState &VertexInputState::operator=( const VertexInputState &copy ) {
		if (!m_id)
		{
			m_id = create_mesh_desc();
		}

		m_container = copy.m_container;
		m_offset = copy.m_offset;
		m_stride = copy.m_stride;
		return *this;
	}

	VertexInputState &VertexInputState::operator=( VertexInputState &&move ) noexcept {
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

	void VertexInputState::set_stride( uint32_t stride ) {
		m_stride = stride;
	}

	void VertexInputState::set_offset( uint32_t offset ) {
		m_offset = offset;
	}

	size_t VertexInputState::get_vertex_size() const {
		size_t attrs_size = 0;
		for (const auto &attr : m_container)
		{
			attrs_size += static_cast<size_t>(attr.size) * sizeof_attribute_type( attr.type );
		}
		return attrs_size;
	}

	void VertexInputState::_validate_attributes() {
		if (m_container.size() > MaxVertexAttributes)
		{
			m_container.resize( MaxVertexAttributes );
		}
	}

}
