# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_CALCULUS_GRADIENT_HPP
# define VCCC_MATH_CALCULUS_GRADIENT_HPP
#
# include "vccc/math/calculus/partial_diff.hpp"

namespace vccc{

//! @addtogroup calculus
//! @{

//! @cond ignored
namespace detail{ namespace math{
template<typename T, typename DifferentialCategory, typename Func, typename VarTuple, std::size_t ...I, typename ...Args>
auto
gradientImpl(Func f, VarTuple vars, std::index_sequence<I...>, Args&&... args)
{
  return std::make_tuple(
      partialDiff<T, I>(DifferentialCategory{},
                        f,
                        vars,
                        std::forward<Args>(args)...)...);
}
}}
//! @endcond


/**
@brief calculates gradient value of the given function and variables
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
auto
gradient(Func f, std::tuple<Vars...> vars, Args&&... args)
{
  static_assert(!vccc::are_<std::is_reference<Vars>...>::value, "tuple element of vars must not be a reference vccc::gradient<>");
  static_assert(vccc::are_<std::is_same<T, Vars>...>::value, "tuple element of vars type must be same with T vccc::gradient<>");
  return detail::math::gradientImpl<T, DifferentialCategory>(
      f,
      vars,
      std::index_sequence_for<Vars...>{},
      std::forward<Args>(args)...);
}

//! @} calculus

}



# endif //VCCC_MATH_CALCULUS_GRADIENT_HPP
