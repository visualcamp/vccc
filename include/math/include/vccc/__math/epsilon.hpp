# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_EPSILON_HPP
# define VCCC_MATH_EPSILON_HPP
#
# include <cmath>
# include <limits>

namespace vccc {

/// @addtogroup math
/// @{

/**
 @brief Get machine epsilon for the given type

 `std::numeric_limits<T>::epsilon()` can't be directly used in a mathematical
 equations. <br>
 Machine epsilon is an upper bound on the relative approximation error due to
 the rounding in floating point arithmetic.

 @tparam T type
 @return machine epsilon of T

 \f$ {\large \sqrt[3]{e} }\f$
 */

// TODO: rename this to be meant 'machine epsilon for numerical derivation'
template<typename T>
inline T epsilon() {
  static const auto e = static_cast<T>(std::cbrt(std::numeric_limits<T>::epsilon()));
  return e;
}

/**
 * @brief Increase the value by small amount
 *
 * @tparam T
 * @tparam U
 * @param x
 * @param eps
 * @return
 */

template<std::size_t i,
    typename Tuple, typename T>
constexpr inline auto
addEpsilon(Tuple vars, T epsilon)
{
  std::get<i>(vars) += std::get<i>(vars) == 0 ? epsilon : std::get<i>(vars) * epsilon;
  return vars;
}

//! @} math

} // namespace vccc

# endif // VCCC_MATH_EPSILON_HPP
