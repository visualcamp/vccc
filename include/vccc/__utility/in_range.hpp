//
// Created by YongGyu Lee on 5/8/24.
//

#ifndef VCCC_UTILITY_IN_RANGE_HPP_
#define VCCC_UTILITY_IN_RANGE_HPP_

#include <limits>

#include "vccc/__utility/cmp.hpp"

namespace vccc {

/// @addtogroup utility
/// @{

template<typename R, typename T>
constexpr bool in_range(T t) noexcept {
  return vccc::cmp_greater_equal(t, std::numeric_limits<R>::min()) &&
         vccc::cmp_less_equal(t, std::numeric_limits<R>::max());
}

/// @}

} // namespace vccc

#endif // VCCC_UTILITY_IN_RANGE_HPP_
