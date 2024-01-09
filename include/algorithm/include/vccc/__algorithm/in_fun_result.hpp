//
// Created by yonggyulee on 1/10/24.
//

#ifndef VCCC_ALGORITHM_IN_FUN_RESULT_HPP
#define VCCC_ALGORITHM_IN_FUN_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

template<typename I, typename F>
struct in_fun_result {
  VCCC_NO_UNIQUE_ADDRESS I in;
  VCCC_NO_UNIQUE_ADDRESS F fun;

  template<typename I2, typename F2, std::enable_if_t<conjunction<
      convertible_to<const I&, I2>,
      convertible_to<const F&, F2>
  >::value, int> = 0>
  constexpr operator in_fun_result<I2, F2>() const & {
    return {in, fun};
  }

  template<typename I2, typename F2, std::enable_if_t<conjunction<
      convertible_to<const I&, I2>,
      convertible_to<const F&, F2>
  >::value, int> = 0>
  constexpr operator in_fun_result<I2, F2>() && {
    return {std::move(in), std::move(fun)};
  }
};

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_IN_FUN_RESULT_HPP
