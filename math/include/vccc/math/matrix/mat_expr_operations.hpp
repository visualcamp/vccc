# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
# define VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
#
# include "vccc/math/matrix/mat_expression.hpp"

namespace vccc{

template<typename E1, typename E2, int m, int n>
constexpr static bool
operator == (const MatExpression<E1, m, n>& lhs, const MatExpression<E2, m, n>& rhs) {
  for(int i=0; i<lhs.size; ++i)
    if(lhs[i] != rhs[i]) return false;
  return true;
}

template<typename E1, typename E2, int m1, int n1, int m2, int n2>
constexpr static bool
operator == (const MatExpression<E1, m1, n1>& lhs, const MatExpression<E2, m2, n2>& rhs) {
  return false;
}

template<typename E1, typename E2, int m1, int n1, int m2, int n2>
constexpr static bool
operator != (const MatExpression<E1, m1, n1>& lhs, const MatExpression<E1, m2, n2>& rhs) {
  return !(lhs == rhs);
}

}

#endif //VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
