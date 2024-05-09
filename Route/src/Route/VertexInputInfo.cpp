#include "pch.h"
#include "../pch.h"
#include "VertexInputInfo.h"
#include "internal/gl_utils.h"

class VPDSetupRegister
{
public:

  static inline void set_enabled_attrs(const size_t num) {
    if (num > VertexInputInfo::MaxVertexAttributes)
    {
      s_enabled_attrs = VertexInputInfo::MaxVertexAttributes;
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
  VertexInputInfo::VertexInputInfo(uint32_t offset, uint32_t stride, const container_type &attributes)
    : m_offset{ offset }, m_stride{ stride }, m_container{ attributes } {
  }

  VertexInputInfo::VertexInputInfo(const container_type &attributes)
    : VertexInputInfo(0, 0, attributes) {
  }

  VertexInputInfo::VertexInputInfo()
    : m_container{}, m_offset{ 0 }, m_stride{ 0 } {
  }

  VertexInputInfo::~VertexInputInfo() {
  }

  VertexInputInfo::VertexInputInfo(const VertexInputInfo &copy)
    : m_container{ copy.m_container }, m_offset{ copy.m_offset }, m_stride{ copy.m_stride } {
  }

  VertexInputInfo::VertexInputInfo(VertexInputInfo &&move) noexcept
    : m_container{ move.m_container }, m_offset{ move.m_offset }, m_stride{ move.m_stride } {
  }

  VertexInputInfo &VertexInputInfo::operator=(const VertexInputInfo &copy) {
    m_container = copy.m_container;
    m_offset = copy.m_offset;
    m_stride = copy.m_stride;
    return *this;
  }

  VertexInputInfo &VertexInputInfo::operator=(VertexInputInfo &&move) noexcept {
    m_container = move.m_container;
    m_offset = move.m_offset;
    m_stride = move.m_stride;
    return *this;
  }

  void VertexInputInfo::set_stride(uint32_t stride) {
    m_stride = stride;
  }

  void VertexInputInfo::set_offset(uint32_t offset) {
    m_offset = offset;
  }

  size_t VertexInputInfo::get_vertex_size() const {
    size_t attrs_size = 0;
    for (const auto &attr : m_container)
    {
      attrs_size += static_cast<size_t>(attr.size) * gl_rt::sizeof_vertex_input_type(attr.type);
    }
    return attrs_size;
  }

  void VertexInputInfo::_validate_attributes() {
    if (m_container.size() > MaxVertexAttributes)
    {
      m_container.resize(MaxVertexAttributes);
    }
  }

}
