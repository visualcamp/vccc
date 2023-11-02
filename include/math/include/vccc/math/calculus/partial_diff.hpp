# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
# define VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
#
# include <limits>
# include "vccc/math/calculus/detail/apply.hpp"
# include "vccc/math/epsilon.hpp"
# include "vccc/math/matrix.hpp"

namespace vccc {

//struct differential_rk_4 {};

//constexpr differential_newtonian_t differential_newtonian;
//constexpr differential_symmetric_t differential_symmetric;

//! @addtogroup math_calculus
//! @{



//template<std::size_t i, typename MatExpr, typename Epsilon>
//inline constexpr decltype(auto)
//addEpsilon(MatrixBase<MatExpr> vars, Epsilon epsilon) {
//  at<i>(vars) += at<i>(vars) == 0 ? epsilon : at<i>(vars) * epsilon;
//  return vars;
//}
//
//template<typename MatExpr, typename Epsilon>
//constexpr decltype(auto)
//addEpsilon(std::size_t i, MatrixBase<MatExpr> vars, Epsilon epsilon) {
//  vars(i) += vars(i) == 0 ? epsilon : vars(i) * epsilon;
//  return vars;
//}

struct differential_newtonian_t {};
struct differential_symmetric_t {};
struct differential_five_point_stencil_t {};

/**
 * @defgroup math_calculus_partialDiff vccc::partialDiff
 * @brief returns partial differential value of \f$i\f$-th parameter
 *
 * @code
 * auto x = std::make_tuple(1.3, 4.12);
 * auto f = [](double x, double y){ return x*x - x*y; };
 * auto df1 = vccc::partialDiff<double, 0>(differential_symmetric_t, f, x);
 * auto df2 = vccc::partialDiff<double, 1>(differential_symmetric_t, f, x);
 * @endcode
 * @sa epsilon: \copybrief epsilon
 *
 * @{
 */

/** @brief get partial differential value using symmetric method

Note: The given function is called 2 times.

 * @tparam T            calculating type
 * @tparam I            index
 * @param f             differentiating function
 * @param vars          tuple of vars
 * @param args          extra function arguments(non variable)
 * @return              partial differential value
 */
template<typename T, std::size_t I,
         typename Func, typename VarTuple, typename ...Args>
auto partialDiff(differential_symmetric_t, Func f, VarTuple vars, Args&&... args) {
  auto x1 = addEpsilon<I>(vars, epsilon<T>());
  auto x2 = addEpsilon<I>(vars, -epsilon<T>());
  auto dx = std::get<I>(x1) - std::get<I>(x2);

  auto fx1 = internal::math::applyTupleAndVariadics(f, x1, args...);
  auto fx2 = internal::math::applyTupleAndVariadics(f, x2, args...);
  return (fx1 - fx2) / dx;
}

/**
@brief get partial differential value using newtonian method

Note: The given function is called 1 times.
Inaccurate then symmetric method

 * @tparam T            calculating type
 * @tparam I            index
 * @param f             differentiating function
 * @param vars          tuple of vars
 * @param args          extra function arguments(non variable)
 * @return              partial differential value
 */
template<typename T, std::size_t I,
         typename Func, typename VarTuple, typename ...Args>
auto
partialDiff(differential_newtonian_t, Func f, VarTuple vars, Args&&... args)
{
  auto x1 = addEpsilon<I>(vars, epsilon<T>());
  auto x2 = vars;
  auto dx = std::get<I>(x1) - std::get<I>(x2);

  auto fx1 = internal::math::applyTupleAndVariadics(f, x1, args...);
  auto fx2 = internal::math::applyTupleAndVariadics(f, x2, args...);
  return (fx1 - fx2) / dx;
}

/**
@brief get partial differential value using five point stencil method

Note: The given function is called 4 times.

 * @tparam T            calculating type
 * @tparam I            index
 * @param f             differentiating function
 * @param vars          tuple of vars
 * @param args          extra function arguments(non variable)
 * @return              partial differential value
 */
template<typename T, std::size_t I,
    typename Func, typename VarTuple, typename ...Args>
auto
partialDiff(differential_five_point_stencil_t, Func f, VarTuple vars, Args&&... args)
{
  auto x1 = addEpsilon<I>(vars,  2 * epsilon<T>());
  auto x2 = addEpsilon<I>(vars,      epsilon<T>());
  auto x3 = addEpsilon<I>(vars,    - epsilon<T>());
  auto x4 = addEpsilon<I>(vars, -2 * epsilon<T>());

  auto fx1 = internal::math::applyTupleAndVariadics(f, x1, args...);
  auto fx2 = internal::math::applyTupleAndVariadics(f, x2, args...);
  auto fx3 = internal::math::applyTupleAndVariadics(f, x3, args...);
  auto fx4 = internal::math::applyTupleAndVariadics(f, x4, args...);

  // TODO: get real dx
  auto fsum = -fx1 + 8*fx2 - 8*fx3 + fx4;
  if(std::get<I>(vars) == 0)
    return fsum / (epsilon<T>() * 12);
  return fsum / (std::get<I>(vars) * (epsilon<T>() * 12));
}

//! @} math_calculus_partialDiff

//! @} math_calculus

//template<typename T, std::size_t I,
//    typename Func, typename VarTuple, typename ...Args>
//auto
//partialDiff(differential_rk_4, Func f, VarTuple vars, Args&&... args){
//  auto x1 = addEpsilon<I>(vars,  2 * epsilon<T>());
//  auto x2 = addEpsilon<I>(vars,      epsilon<T>());
//  auto x3 = addEpsilon<I>(vars,    - epsilon<T>());
//  auto x4 = addEpsilon<I>(vars, -2 * epsilon<T>());
//
//  auto fx1 = internal::math::applyTupleAndVariadics(f, x1, args...);
//  auto fx2 = internal::math::applyTupleAndVariadics(f, x2, args...);
//  auto fx3 = internal::math::applyTupleAndVariadics(f, x3, args...);
//  auto fx4 = internal::math::applyTupleAndVariadics(f, x4, args...);
//
//  auto fsum = -fx1 + 8*fx2 - 8*fx3 + fx4;
//  if(std::get<I>(vars) == 0)
//    return fsum / (epsilon<T>() * 12);
//  return fsum / (std::get<I>(vars) * (epsilon<T>() * 12));
//}

} // namespace vccc

# endif // VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
