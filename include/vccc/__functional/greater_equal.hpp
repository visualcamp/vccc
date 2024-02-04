//
// Created by YongGyu Lee on 2024/02/04.
//

#ifndef VCCC_FUNCTIONAL_GREATER_EQUAL_HPP
#define VCCC_FUNCTIONAL_GREATER_EQUAL_HPP

#include <type_traits>
#include <utility>

#include "vccc/__concepts/totally_ordered.hpp"
#include "vccc/__functional/less.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup functional
/// @{

struct greater_equal {
  using is_transparent = std::true_type;

  template<typename T, typename U, std::enable_if_t<totally_ordered_with<T, U>::value, int> = 0>
  constexpr bool operator()(T&& t, U&& u) const
      noexcept(noexcept(!ranges::less{}(std::forward<T>(t), std::forward<U>(u))))
  {
    return !ranges::less{}(std::forward<T>(t), std::forward<U>(u));
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_FUNCTIONAL_GREATER_EQUAL_HPP
