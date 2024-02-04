//
// Created by YongGyu Lee on 2024/02/04.
//

#ifndef VCCC_FUNCTIONAL_NOT_EQUAL_TO_HPP
#define VCCC_FUNCTIONAL_NOT_EQUAL_TO_HPP

#include <type_traits>
#include <utility>

#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__functional/equal_to.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup functional
/// @{

struct not_equal_to {
  using is_transparent = std::true_type;

  template<typename T, typename U, std::enable_if_t<equality_comparable_with<T, U>::value, int> = 0>
  constexpr bool operator()(T&& t, U&& u) const
      noexcept(noexcept(!ranges::equal_to{}(std::forward<T>(t), std::forward<U>(u))))
  {
    return !ranges::equal_to{}(std::forward<T>(t), std::forward<U>(u));
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_FUNCTIONAL_NOT_EQUAL_TO_HPP
