//
// Created by yonggyulee on 2023/10/25.
//

#ifndef VCCC_NUMERIC_FLOAT_EQUAL_HPP
#define VCCC_NUMERIC_FLOAT_EQUAL_HPP

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

#include "vccc/math/epsilon.hpp"

namespace vccc {

/**
 * @addtogroup numeric
 * @{
 *      @defgroup numeric_float_equal_to float_equal_to
 *      @defgroup numeric_float_equal float_equal
 * @}
 */

/// @defgroup numeric_float_equal_to
/// @{

namespace detail {

template<typename T, bool v = std::numeric_limits<T>::is_exact>
struct float_equal_to_impl;

template<typename T>
struct float_equal_to_impl<T, true> {
  constexpr inline bool operator()(const T& a, const T& b, const T&) const {
    return a == b;
  }
};

template<typename T>
struct float_equal_to_impl<T, false> {
  constexpr inline bool operator()(const T& a, const T& b, const T& e) const {
    return compare_positive(std::abs(a), std::abs(b), std::abs(a - b), e);
  }

 private:
  constexpr inline bool compare_positive(const T& a, const T& b, const T& diff, const T& e) const {
    return (a <= e && b <= e) || diff <= (std::min)(a, b) * e;
  }
};

} // namespace detail

/**
 * @brief function object implementing approximately equal of x and y
 * @sa float_equal_to<void>: \copybrief float_equal_to<void>
 * @sa float_equal: \copybrief float_equal
 */
template<typename T = void>
struct float_equal_to {
  constexpr inline bool operator()(const T& a, const T& b, const T& e = vccc::epsilon<T>()) const {
    return detail::float_equal_to_impl<T>{}(a, b, e);
  }
};


/**
 * float_equal_to<void>
 * @brief A specialization of vccc::float_equal_to with parameter and return type deduced.
 * @sa float_equal_to: \copybrief float_equal_to
 * @sa float_equal: \copybrief float_equal
 */
template<>
struct float_equal_to<void> {
  template<typename T, typename U, typename E = std::common_type_t<T, U>>
  constexpr inline auto operator()(const T& a, const U& b, const E& e = vccc::epsilon<E>()) const
      -> decltype(std::declval<float_equal_to<std::common_type_t<T, U, E>>&>()(std::declval<const T&>(), std::declval<const U&>(), std::declval<const E&>()))
  {
    return float_equal_to<std::common_type_t<T, U, E>>{}(a, b, e);
  }
};

/// @} numeric_float_equal_to

/// @addtogroup numeric_float_equal
/// @{

/**
 * @brief Compare if two floating-points are approximately equal
 *
 * Default epsilon values are `4.92157e-03f` for `float` and `6.0554545e-06` for `double` type. <br>
 * The values must be representable
 *
 * Return `true` if both values are smaller or same with `epsilon`. <br>
 * Otherwise, return true if `diff <= std::min(std::abs(a), std::abs(b)) * epsilon`, false otherwise <br>
 * where `diff` is `std::abs(a - b)`.
 *
 * @sa float_equal_to: \copybrief float_equal_to
 *
 * @tparam T
 * @param a
 * @param b
 * @param epsilon
 * @return
 */
template<typename T, typename U, typename E = std::common_type_t<T, U>>
constexpr inline bool
float_equal(const T& a, const U& b, const E& epsilon = vccc::epsilon<E>()) {
  return float_equal_to<std::common_type_t<T, U, E>>{}(a, b, epsilon);
}

/// @} numeric_float_equal

} // namespace vccc

#endif // VCCC_NUMERIC_FLOAT_EQUAL_HPP
