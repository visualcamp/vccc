# /*
#  * Created by YongGyu Lee on 2020/02/06.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_INV_HPP
# define VCCC_MATH_MATRIX_MATRIX_INV_HPP
#
# include "vccc/math/matrix/mat_expression.hpp"

namespace vccc {

template<typename E>
class MatrixInverter {
  const E& e;
 public:
  MatrixInverter(const E& e) : e(e) {
    static_assert(E::cols == E::rows, "Cannot invert non-square matrix");
  }

};

}

# endif //VCCC_MATH_MATRIX_MATRIX_INV_HPP
