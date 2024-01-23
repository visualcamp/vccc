# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_GRADIENT_HPP
# define VCCC_MATH_GRADIENT_HPP
#
# include "vccc/__math/partial_diff.hpp"
# include "vccc/type_traits.hpp"

namespace vccc {

/**
 * @addtogroup math
 * @{
 * @addtogroup math_gradient__func__calculus gradient
 * @brief calculates gradient value of the given function and variables
 * @sa partialDiff: \copybrief partialDiff
 * @{
 */

namespace internal {
namespace math {
template<typename T, typename DifferentialCategory, typename Func, typename VarTuple, std::size_t ...I, typename ...Args>
inline auto gradientImpl(Func f, VarTuple vars, std::index_sequence<I...>, Args&&... args) {
  return std::make_tuple(
      partialDiff<T, I>(DifferentialCategory{}, f, vars, std::forward<Args>(args)...)...);
}
} // namespace math
} // namespace internal


/**
 *
 * @tparam T                        calculating type
 * @tparam DifferentialCategory     differentiating method
 * @param f         function
 * @param vars      function variables
 * @param args      non-variable extra function parameters
 * @return
 */
template<typename T, typename DifferentialCategory = differential_symmetric_t,
    typename Func, typename ...Vars, typename ...Args>
inline auto gradient(Func f, std::tuple<Vars...> vars, Args&&... args) {
  static_assert(negation<disjunction<std::is_reference<Vars>...>>::value,
                "tuple element of vars must not be a reference vccc::gradient<>");
  static_assert(conjunction<std::is_same<T, Vars>...>::value,
                "tuple element of vars type must be same with T vccc::gradient<>");
  return internal::math::gradientImpl<T, DifferentialCategory>(
      f,
      vars,
      std::index_sequence_for<Vars...>{},
      std::forward<Args>(args)...);
}

//! @}
/// @}

} // namespace vccc

# endif // VCCC_MATH_GRADIENT_HPP
