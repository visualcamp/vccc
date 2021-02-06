# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUB_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUB_HPP
#
# include "vccc/math/algebra/matrix/mat_expression.hpp"

namespace vccc {

template<typename E1, typename E2, typename Category, int m, int n>
class MatrixSub : public MatExpression<MatrixSub<E1, E2, Category, m, n>, m, n> {
  const E1& e1;
  const E2& e2;

  constexpr inline auto get_inner(std::size_t i, matrix_sub_normal_t) const;
  constexpr inline auto get_inner(std::size_t i, matrix_sub_unary_t) const;

  constexpr inline auto get_inner(std::size_t i, std::size_t j, matrix_sub_normal_t) const;
  constexpr inline auto get_inner(std::size_t i, std::size_t j, matrix_sub_unary_t) const;

 public:
  constexpr inline MatrixSub(const E1& e1, const E2& e2);

  constexpr inline decltype(auto) operator() (std::size_t i) const;
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const;
  constexpr inline decltype(auto) operator[] (std::size_t i) const;
};

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline auto
MatrixSub<E1, E2, Category, m, n>::get_inner(std::size_t i, matrix_sub_normal_t) const {
  return e1[i] - e2[i];
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline auto
MatrixSub<E1, E2, Category, m, n>::get_inner(std::size_t i, matrix_sub_unary_t) const {
  return -e1[i];
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline auto
MatrixSub<E1, E2, Category, m, n>::get_inner(std::size_t i, std::size_t j, matrix_sub_normal_t) const {
  return e1(i, j) - e2(i, j);
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline auto
MatrixSub<E1, E2, Category, m, n>::get_inner(std::size_t i, std::size_t j, matrix_sub_unary_t) const {
  return -e1(i, j);
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr MatrixSub<E1, E2, Category, m, n>::MatrixSub(const E1& e1, const E2& e2) : e1(e1), e2(e2) {
  static_assert(E1::cols == E2::cols && E1::rows == E2::rows, "Substituting matrix's size must be same");
}

template<typename E1, typename E2, int m, int n>
constexpr
inline
MatrixSub<E1, E2, matrix_sub_normal_t, m, n>
operator - (const MatExpression<E1, m, n>& lhs, const MatExpression<E2, m, n>& rhs) {
  return MatrixSub<E1, E2, matrix_sub_normal_t, m, n>(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(rhs));
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline decltype(auto)
MatrixSub<E1, E2, Category, m, n>::operator() (std::size_t i) const {
  return get_inner(i, Category{});
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline decltype(auto)
MatrixSub<E1, E2, Category, m, n>::operator() (std::size_t i, std::size_t j) const {
  return get_inner(i, j, Category{});
}

template<typename E1, typename E2, typename Category, int m, int n>
constexpr inline decltype(auto)
MatrixSub<E1, E2, Category, m, n>::operator[] (std::size_t i) const {
  return get_inner(i, Category{});
}

}



# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_SUB_HPP
