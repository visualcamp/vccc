//
// Created by yonggyulee on 1/25/24.
//

#ifndef VCCC_ALGORITHM_IN_FOUND_RESULT_HPP
#define VCCC_ALGORITHM_IN_FOUND_RESULT_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/no_unique_address.hpp"
#include "vccc/__concepts/convertible_to.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I>
struct in_found_result {
  VCCC_NO_UNIQUE_ADDRESS I in;
  VCCC_NO_UNIQUE_ADDRESS bool found;

  template<typename I2, std::enable_if_t<
      convertible_to<const I&, I2>::value, int> = 0>
  constexpr operator in_found_result<I2>() const & {
    return {in, found};
  }

  template<typename I2, std::enable_if_t<
      convertible_to<I, I2>::value, int> = 0>
  constexpr operator in_found_result<I2>() && {
    return {std::move(in), found};
  }
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_IN_FOUND_RESULT_HPP
