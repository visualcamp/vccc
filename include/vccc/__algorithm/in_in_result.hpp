//
// Created by yonggyulee on 1/25/24.
//

#ifndef VCCC_ALGORITHM_IN_IN_RESULT_HPP
#define VCCC_ALGORITHM_IN_IN_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I1, typename I2>
struct in_in_result {
  VCCC_NO_UNIQUE_ADDRESS I1 in1;
  VCCC_NO_UNIQUE_ADDRESS I2 in2;

  template<typename II1, typename II2, std::enable_if_t<conjunction<
      convertible_to<const I1&, II1>,
      convertible_to<const I2&, II2>
  >::value, int> = 0>
  constexpr operator in_in_result<I2, II2>() const & {
    return {in1, in2};
  }

  template<typename II1, typename II2, std::enable_if_t<conjunction<
      convertible_to<I1, II1>,
      convertible_to<I2, II2>
  >::value, int> = 0>
  constexpr operator in_in_result<I2, II2>() && {
    return {std::move(in1), std::move(in2)};
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_IN_IN_RESULT_HPP
