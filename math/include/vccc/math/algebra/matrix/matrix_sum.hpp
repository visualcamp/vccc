# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUM_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUM_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"

namespace vccc {

template<typename E1, typename E2, int m, int n>
class MatrixSum : public MatExpression<MatrixSum<E1, E2, m, n>, m, n> {
  const E1& e1;
  const E2& e2;

 public:
  constexpr inline MatrixSum(const E1& e1, const E2& e2);

  // operator() (std::size_t)
  constexpr inline decltype(auto) operator() (std::size_t i) const { return e1(i) + e2(i); }
  constexpr inline decltype(auto) operator() (std::size_t i) { return e1(i) + e2(i); }

  // operator() (std::size_t, std::size_t)
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const { return e1(i, j) + e2(i, j); }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) { return e1(i, j) + e2(i, j); }

  // operator[] (std::size_t)
  constexpr inline decltype(auto) operator[] (std::size_t i) const { return e1[i] + e2[i]; }
  constexpr inline decltype(auto) operator[] (std::size_t i) { return e1[i] + e2[i]; }

};

template<typename E1, typename E2, int m, int n>
constexpr
MatrixSum<E1, E2, m, n>::MatrixSum(const E1& e1, const E2& e2) : e1(e1), e2(e2) {
  static_assert(E1::cols == E2::cols && E1::rows == E2::rows, "each matrix's size must be same");
}

template<typename E1, typename E2, int m, int n>
constexpr
inline
MatrixSum<E1, E2, m, n>
operator + (const MatExpression<E1, m, n>& lhs, const MatExpression<E2, m, n>& rhs) {
  return MatrixSum<E1, E2, m, n>(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs));
}

}

# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUM_HPP
