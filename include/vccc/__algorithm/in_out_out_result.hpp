//
// Created by yonggyulee on 1/25/24.
//

#ifndef VCCC_ALGORITHM_IN_OUT_OUT_RESULT_HPP
#define VCCC_ALGORITHM_IN_OUT_OUT_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I, typename O1, typename O2>
struct in_out_out_result {
  VCCC_NO_UNIQUE_ADDRESS I in;
  VCCC_NO_UNIQUE_ADDRESS O1 out1;
  VCCC_NO_UNIQUE_ADDRESS O2 out2;

  template<typename II, typename OO1, typename OO2, std::enable_if_t<conjunction<
      convertible_to<const I&, II>,
      convertible_to<const O1&, OO1>,
      convertible_to<const O2&, OO2>
  >::value, int> = 0>
  constexpr operator in_out_out_result<II, OO1, OO2>() const & {
    return {in, out1, out2};
  }

  template<typename II, typename OO1, typename OO2, std::enable_if_t<conjunction<
      convertible_to<I, II>,
      convertible_to<O1, OO1>,
      convertible_to<O2, OO2>
  >::value, int> = 0>
  constexpr operator in_out_out_result<II, OO1, OO2>() && {
    return {std::move(in), std::move(out1), std::move(out2)};
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_IN_OUT_OUT_RESULT_HPP
