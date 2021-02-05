# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_OSTREAM_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_OSTREAM_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"
# include <ostream>

namespace vccc {

template<typename E, int m, int n>
std::ostream& operator << (std::ostream& os, const MatExpression<E, m, n>& mat_expr) {
  os << '[';

  os << mat_expr(0, 0);
  for(int j=1; j<n; ++j) {
    os << ", " << mat_expr(0, j);
  }
  os << ";";

  for(int i=1; i<m; ++i){
    os << "\n " << mat_expr(i, 0);
    for(int j=1; j<n; ++j) {
      os << ", " << mat_expr(i, j);
    }
    os << ";";
  }

  os << ']';
  return os;
}

}

# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_OSTREAM_HPP
