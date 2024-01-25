//
// Created by yonggyulee on 1/25/24.
//

#ifndef VCCC_ALGORITHM_OUT_VALUE_RESULT_HPP
#define VCCC_ALGORITHM_OUT_VALUE_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I, typename T>
struct out_value_result {
  VCCC_NO_UNIQUE_ADDRESS I out;
  VCCC_NO_UNIQUE_ADDRESS T value;

  template<typename I2, typename T2, std::enable_if_t<conjunction<
      convertible_to<const I&, I2>,
      convertible_to<const T&, T2>
  >::value, int> = 0>
  constexpr operator out_value_result<I2, T2>() const & {
    return {out, value};
  }

  template<typename I2, typename T2, std::enable_if_t<conjunction<
      convertible_to<I, I2>,
      convertible_to<T, T2>
  >::value, int> = 0>
  constexpr operator out_value_result<I2, T2>() && {
    return {std::move(out), std::move(value)};
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_OUT_VALUE_RESULT_HPP
