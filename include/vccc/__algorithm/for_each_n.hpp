//
// Created by YongGyu Lee on 1/25/24.
//

#ifndef VCCC_ALGORITHM_FOR_EACH_N_HPP
#define VCCC_ALGORITHM_FOR_EACH_N_HPP

#include <type_traits>

#include "vccc/__iterator/iterator_traits/legacy_input_iterator.hpp"

namespace vccc {

/// @addtogroup algorithm
/// @{

template<typename InputIt, typename Size, typename UnaryFunction>
constexpr InputIt for_each_n(InputIt first, Size n, UnaryFunction f) {
  static_assert(LegacyInputIterator<InputIt>::value, "Constraints not satisfied");
  static_assert(std::is_move_constructible<InputIt>::value, "Constraints not satisfied");

  for (Size i = 0; i < n; ++first, (void) ++i) {
    f(*first);
  }

  return first;
}

/// @}

} // namespace vccc

#endif // VCCC_ALGORITHM_FOR_EACH_N_HPP
