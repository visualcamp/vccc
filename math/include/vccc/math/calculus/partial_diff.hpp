# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
# define VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
#
# include <limits>
# include "vccc/math/calculus/detail/apply.hpp"
# include "vccc/math/calculus/epsilon.hpp"

namespace vccc{

struct differential_newtonian_t {};
struct differential_symmetric_t {};
struct differential_five_point_stencil_t {};
//struct differential_rk_4 {};

//constexpr differential_newtonian_t differential_newtonian;
//constexpr differential_symmetric_t differential_symmetric;

template<std::size_t i,
         typename Tuple, typename T>
auto
addEpsilon(Tuple vars, volatile T epsilon) {
  std::get<i>(vars) += std::get<i>(vars) == 0 ? epsilon : std::get<i>(vars) * epsilon;
  return vars;
}

template<typename T, std::size_t I,
         typename Func, typename VarTuple, typename ...Args>
auto
partialDiff(differential_symmetric_t, Func f, VarTuple vars, Args&&... args){
  auto x1 = addEpsilon<I>(vars, epsilon<T>());
  auto x2 = addEpsilon<I>(vars, -epsilon<T>());
  auto fx1 = detail::math::applyTupleAndVariadics(f, x1, args...);
  auto fx2 = detail::math::applyTupleAndVariadics(f, x2, args...);
  if(std::get<I>(vars) == 0)
    return (fx1 - fx2) / (epsilon<T>() * 2);
  return (fx1 - fx2) / (std::get<I>(vars) * (epsilon<T>() * 2));
}

template<typename T, std::size_t I,
         typename Func, typename VarTuple, typename ...Args>
auto
partialDiff(differential_newtonian_t, Func f, VarTuple vars, Args&&... args){
  auto x1 = addEpsilon<I>(vars, epsilon<T>());
  auto x2 = vars;
  auto fx1 = detail::math::applyTupleAndVariadics(f, x1, args...);
  auto fx2 = detail::math::applyTupleAndVariadics(f, x2, args...);
  if(std::get<I>(vars) == 0)
    return (fx1 - fx2) / (epsilon<T>() * 2);
  return (fx1 - fx2) / (std::get<I>(vars) * epsilon<T>());
}

template<typename T, std::size_t I,
    typename Func, typename VarTuple, typename ...Args>
auto
partialDiff(differential_five_point_stencil_t, Func f, VarTuple vars, Args&&... args){
  auto x1 = addEpsilon<I>(vars,  2 * epsilon<T>());
  auto x2 = addEpsilon<I>(vars,      epsilon<T>());
  auto x3 = addEpsilon<I>(vars,    - epsilon<T>());
  auto x4 = addEpsilon<I>(vars, -2 * epsilon<T>());

  auto fx1 = detail::math::applyTupleAndVariadics(f, x1, args...);
  auto fx2 = detail::math::applyTupleAndVariadics(f, x2, args...);
  auto fx3 = detail::math::applyTupleAndVariadics(f, x3, args...);
  auto fx4 = detail::math::applyTupleAndVariadics(f, x4, args...);

  auto fsum = -fx1 + 8*fx2 - 8*fx3 + fx4;
  if(std::get<I>(vars) == 0)
    return fsum / (epsilon<T>() * 12);
  return fsum / (std::get<I>(vars) * (epsilon<T>() * 12));
}


//template<typename T, std::size_t I,
//    typename Func, typename VarTuple, typename ...Args>
//auto
//partialDiff(differential_rk_4, Func f, VarTuple vars, Args&&... args){
//  auto x1 = addEpsilon<I>(vars,  2 * epsilon<T>());
//  auto x2 = addEpsilon<I>(vars,      epsilon<T>());
//  auto x3 = addEpsilon<I>(vars,    - epsilon<T>());
//  auto x4 = addEpsilon<I>(vars, -2 * epsilon<T>());
//
//  auto fx1 = detail::math::applyTupleAndVariadics(f, x1, args...);
//  auto fx2 = detail::math::applyTupleAndVariadics(f, x2, args...);
//  auto fx3 = detail::math::applyTupleAndVariadics(f, x3, args...);
//  auto fx4 = detail::math::applyTupleAndVariadics(f, x4, args...);
//
//  auto fsum = -fx1 + 8*fx2 - 8*fx3 + fx4;
//  if(std::get<I>(vars) == 0)
//    return fsum / (epsilon<T>() * 12);
//  return fsum / (std::get<I>(vars) * (epsilon<T>() * 12));
//}

}

# endif //VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
