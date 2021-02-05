# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MAT_EXPR_OPERATIONS_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MAT_EXPR_OPERATIONS_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"

namespace vccc{

template<typename E1, typename E2, int m, int n>
constexpr bool
operator == (const MatExpression<E1, m, n>& lhs, const MatExpression<E2, m, n>& rhs) {
  for(int i=0; i<lhs.size; ++i)
    if(lhs[i] != rhs[i]) return false;
  return true;
}

template<typename E1, typename E2, int m1, int n1, int m2, int n2>
constexpr bool
operator == (const MatExpression<E1, m1, n1>& lhs, const MatExpression<E2, m2, n2>& rhs) {
  return false;
}

template<typename E1, typename E2, int m1, int n1, int m2, int n2>
constexpr bool
operator != (const MatExpression<E1, m1, n1>& lhs, const MatExpression<E1, m2, n2>& rhs) {
  return !(lhs == rhs);
}


//template<typename E1, typename E2>
//constexpr bool
//operator != (const MatExpression<E1>& lhs, const MatExpression<E2>& rhs) {
//  return !(lhs == rhs);
//}
//
//template<typename E1, typename E2>
//constexpr
//inline
//decltype(auto)
//operator += (MatExpression<E1>& lhs, const MatExpression<E2>& rhs) {
//  static_assert(lhs.cols() == rhs.cols() && lhs.rows() == rhs.rows(),
//                "Matrix size must be same. vccc::operator += (MatExpression<E1>&, const MatExpression<E2>&)");
//  for(int i=0; i<lhs.size(); ++i)
//    lhs[i] += rhs[i];
//  return lhs;
//}
//
//template<typename E1, typename E2>
//constexpr
//inline
//decltype(auto)
//operator -= (MatExpression<E1>& lhs, const MatExpression<E2>& rhs) {
//  static_assert(lhs.cols() == rhs.cols() && lhs.rows() == rhs.rows(),
//                "Matrix size must be same. vccc::operator += (MatExpression<E1>&, const MatExpression<E2>&)");
//  for(int i=0; i<lhs.size(); ++i)
//    lhs[i] -= rhs[i];
//  return lhs;
//}
//
//template<typename E, typename T>
//constexpr
//inline
//decltype(auto)
//operator *= (MatExpression<E>& lhs, const T& value) {
//  for(int i=0; i<lhs.size(); ++i)
//    lhs[i] *= value;
//  return lhs;
//}
//
//template<typename E, typename T>
//constexpr
//inline
//decltype(auto)
//operator /= (MatExpression<E>& lhs, const T& value) {
//  for(int i=0; i<lhs.size(); ++i)
//    lhs[i] /= value;
//  return lhs;
//}

}

#endif //VCCC_MATH_ALGEBRA_MATRIX_MAT_EXPR_OPERATIONS_HPP
