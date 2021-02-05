# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUB_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUB_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"
# include "vccc/type_traits.hpp"
# include <type_traits>

namespace vccc {

struct matrix_sub_normal {};
struct matrix_sub_unary {};

template<typename E1, typename E2, typename Category, int m, int n>
class MatrixSub : public MatExpression<MatrixSub<E1, E2, Category, m, n>, m, n> {
  const E1& e1;
  const E2& e2;

  constexpr inline auto get_inner(std::size_t i, matrix_sub_normal) const { return e1[i] - e2[i]; }
  constexpr inline auto get_inner(std::size_t i, matrix_sub_unary) const { return -e1[i]; }

 public:
  constexpr inline MatrixSub(const E1& e1, const E2& e2);

  constexpr inline decltype(auto) operator[] (std::size_t i) const { return get_inner(i, Category{}); }
};

template<typename E1, typename E2, typename Category, int m, int n>
constexpr MatrixSub<E1, E2, Category, m, n>::MatrixSub(const E1& e1, const E2& e2) : e1(e1), e2(e2) {
  static_assert(e1.cols == e2.cols && e1.rows == e2.rows, "Substituting matrix's size must be same");
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr
inline
MatrixSub<E1, E2, Category, m, n>
operator - (const MatExpression<E1, m, n>& lhs, const MatExpression<E2, m, n>& rhs) {
  return MatrixSub<E1, E2, matrix_sub_normal, m, n>(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(rhs));
}

template<typename E, typename Category, int m, int n>
constexpr
inline
MatrixSub<E, E, Category, m, n>
operator - (const MatExpression<E, m, n>& lhs) {
  return MatrixSub<E, E, matrix_sub_unary, m, n>(*static_cast<const E*>(&lhs), *static_cast<const E*>(lhs));
}


}



# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUB_HPP
