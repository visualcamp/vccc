# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_OSTREAM_HPP
# define VCCC_MATH_MATRIX_MATRIX_OSTREAM_HPP
#
# include "vccc/__math/matrix/matrix_base.hpp"
# include <ostream>

namespace vccc {

//! @addtogroup math_matrix
//! @{

template<typename E>
std::ostream& operator << (std::ostream& os, const MatrixBase<E>& mat_expr) {
  os << '[';

  os << mat_expr(0, 0);
  for(int j=1; j<mat_expr.cols; ++j) {
    os << ", " << mat_expr(0, j);
  }
  os << ";";

  for(int i=1; i<mat_expr.rows; ++i){
    os << "\n " << mat_expr(i, 0);
    for(int j=1; j<mat_expr.cols; ++j) {
      os << ", " << mat_expr(i, j);
    }
    os << ";";
  }

  os << ']';
  return os;
}

//! @} math_matrix

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_OSTREAM_HPP
