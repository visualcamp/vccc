//
// Created by YongGyu Lee on 2024/01/12.
//

#ifndef VCCC_FUNCTIONAL_EQUAL_TO_HPP
#define VCCC_FUNCTIONAL_EQUAL_TO_HPP

#include <type_traits>
#include <utility>

#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup functional
/// @{

struct equal_to {
  using is_transparent = std::true_type;

  template<typename T, typename U, std::enable_if_t<equality_comparable_with<T, U>::value, int> = 0>
  constexpr bool operator()(T&& t, U&& u) const
      noexcept(noexcept(bool(std::forward<T>(t) == std::forward<U>(u))))
  {
    return std::forward<T>(t) == std::forward<U>(u);
  }
};


/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_FUNCTIONAL_EQUAL_TO_HPP
