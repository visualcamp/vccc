# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_MUL_SCALAR_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_MUL_SCALAR_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"
# include <type_traits>

namespace vccc{

template<typename E, typename T, int m, int n>
class MatrixMulScalar : public MatExpression<MatrixMulScalar<E, T, m, n>, m, n> {
  const E& e;
  const T& value;

 public:
  constexpr MatrixMulScalar(const E& e, const T& value);

  constexpr inline decltype(auto) operator() (std::size_t i) const { return e(i) * value; }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const { return e(i, j) * value; }
  constexpr inline decltype(auto) operator[] (std::size_t i) const { return e[i] * value; }

};

template<typename E, typename T, int m, int n>
constexpr MatrixMulScalar<E, T, m, n>::MatrixMulScalar(const E& e, const T& value) : e(e), value(value) {}

template<typename E, typename T, int m, int n, std::enable_if_t<!is_matrix<T>::value, int> = 0>
constexpr
inline
MatrixMulScalar<E, T, m, n>
operator * (const MatExpression<E, m, n>& lhs, const T& value) {
  return MatrixMulScalar<E, T, m, n>(*static_cast<const E*>(&lhs), value);
}

template<typename E, typename T, int m, int n>
constexpr
inline
MatrixMulScalar<E, T, m, n>
operator / (const MatExpression<E, m, n>& lhs, const T& value) {
  static_assert(!is_matrix<T>::value, "Matrix cannot divide other matrix.");
  return MatrixMulScalar<E, T, m, n>(*static_cast<const E*>(&lhs), T(1) / value);
}


}

# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_MUL_SCALAR_HPP
