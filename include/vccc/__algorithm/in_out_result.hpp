//
// Created by yonggyulee on 1/12/24.
//

#ifndef VCCC_ALGORITHM_IN_OUT_RESULT_HPP
#define VCCC_ALGORITHM_IN_OUT_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I, typename O>
struct in_out_result {
  VCCC_NO_UNIQUE_ADDRESS I in;
  VCCC_NO_UNIQUE_ADDRESS O out;

  template<typename I2, typename O2, std::enable_if_t<conjunction<
      convertible_to<const I&, I2>,
      convertible_to<const O&, O2>
  >::value, int> = 0>
  constexpr operator in_out_result<I2, O2>() const & {
    return {in, out};
  }

  template<typename I2, typename O2, std::enable_if_t<conjunction<
      convertible_to<I, I2>,
      convertible_to<O, O2>
  >::value, int> = 0>
  constexpr operator in_out_result<I2, O2>() && {
    return {std::move(in), std::move(out)};
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_IN_OUT_RESULT_HPP
