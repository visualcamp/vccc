//
// Created by YongGyu Lee on 2024/02/04.
//

#ifndef VCCC_FUNCTIONAL_GREATER_HPP
#define VCCC_FUNCTIONAL_GREATER_HPP

#include <type_traits>
#include <utility>

#include "vccc/__concepts/totally_ordered.hpp"
#include "vccc/__functional/less.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup functional
/// @{

struct greater {
  using is_transparent = std::true_type;

  template<typename T, typename U, std::enable_if_t<totally_ordered_with<T, U>::value, int> = 0>
  constexpr bool operator()(T&& t, U&& u) const
      noexcept(noexcept(ranges::less{}(std::forward<U>(u), std::forward<T>(t))))
  {
    return ranges::less{}(std::forward<U>(u), std::forward<T>(t));
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_FUNCTIONAL_GREATER_HPP
