#include "pch.h"
#include "../pch.h"
#include "StorageBuffer.h"
#include "IMap.h"

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

}
