#pragma once
#include "Bases.h"

namespace route
{
  /// @brief inclusive start, exclusive end range
  /// @tparam _Ty the range value type
  template <typename _Ty>
  struct TRange
  {
    static_assert(std::is_arithmetic_v<_Ty> || std::is_pointer_v<_Ty>, "only arithmetic or pointer ranges are allowed");

    inline constexpr TRange(_Ty pstart, _Ty pend) : start{ pstart }, end{ pend } {
    }

    inline constexpr _Ty length() const {
      return end - start;
    }

    inline constexpr bool valid() const {
      return end > start;
    }

    inline constexpr bool contains(const _Ty value) const {
      return value < end && value >= start;
    }

    _Ty start, end;
  };

  using FloatRange = TRange<float>;
  using IntRange = TRange<int>;
  using IndexRange = TRange<index_t>;

}
