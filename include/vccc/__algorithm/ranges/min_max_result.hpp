//
// Created by yonggyulee on 1/25/24.
//

#ifndef VCCC_ALGORITHM_RANGES_MIN_MAX_RESULT_HPP
#define VCCC_ALGORITHM_RANGES_MIN_MAX_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename T>
struct min_max_result {
  VCCC_NO_UNIQUE_ADDRESS T min;
  VCCC_NO_UNIQUE_ADDRESS T max;

  template<typename T2, std::enable_if_t<
      convertible_to<const T&, T2>::value, int> = 0>
  constexpr operator min_max_result<T2>() const & {
    return {min, max};
  }

  template<typename T2, std::enable_if_t<
      convertible_to<T, T2>::value, int> = 0>
  constexpr operator min_max_result<T2>() && {
    return {std::move(min), std::move(max)};
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_MIN_MAX_RESULT_HPP
