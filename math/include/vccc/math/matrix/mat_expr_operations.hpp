# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
# define VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
#
# include "vccc/math/matrix/matrix_base.hpp"

namespace vccc{

template<typename E1, typename E2>
constexpr bool
operator == (const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) {
  if(lhs.rows != rhs.rows || lhs.cols != rhs.cols)
    return false;
  for(int i=0; i<lhs.size; ++i)
    if(lhs(i) != rhs(i)) return false;
  return true;
}

template<typename E1, typename E2>
constexpr bool
operator != (const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) {
  return !(lhs == rhs);
}

}

#endif //VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
