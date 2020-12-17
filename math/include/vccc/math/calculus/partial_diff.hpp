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

//constexpr differential_newtonian_t differential_newtonian;
//constexpr differential_symmetric_t differential_symmetric;

template<std::size_t i,
         typename Tuple, typename T>
auto
addEpsilon(Tuple vars, volatile T epsilon) {
  std::get<i>(vars) += std::get<i>(vars) * epsilon;
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

}

# endif //VCCC_MATH_CALCULUS_PARTIAL_DIFF_HPP
