# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
# define VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
#
# include "vccc/__math/matrix/matrix_base.hpp"
# include "vccc/__math/matrix/type_helper.hpp"

namespace vccc {

template<typename E1, typename E2, std::enable_if_t<internal::math::is_same_size<E1, E2>::value, int> = 0>
constexpr bool
operator == (const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) {
  for(int i=0; i<lhs.size; ++i)
    if(lhs(i) != rhs(i)) return false;
  return true;
}

template<typename E1, typename E2, std::enable_if_t<internal::math::is_same_size<E1, E2>::value, int> = 0>
constexpr bool
operator != (const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) {
  return !(lhs == rhs);
}

} // namespace vccc

#endif // VCCC_MATH_MATRIX_MAT_EXPR_OPERATIONS_HPP
