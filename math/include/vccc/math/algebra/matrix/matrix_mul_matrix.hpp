# /*
#  * Created by YongGyu Lee on 2020/02/06.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_MATRIX_MUL_MATRIX_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_MATRIX_MUL_MATRIX_HPP
#
# include "vccc/math/algebra/matrix/forward_declare.hpp"
# include <type_traits>

namespace vccc{

template<typename E1, typename E2, int m, int l, int n>
constexpr static inline auto
operator * (const MatExpression<E1, m, l>& lhs, const MatExpression<E2, l, n>& rhs);

template<typename E1, typename E2, int m, int l, int n>
class MatrixMulMatrix {
  template<int p, int q, int r> struct tag {};

 public:
  static_assert(E1::cols == E2::rows, "lhs' cols must be same with rhs' rows when multiplying matrices.");
  static_assert(m > 0 && l > 0 && n > 0, "Matrix size must be bigger than 0");

  using value_type = typename E1::value_type;
  using return_type = Matrix<value_type, m, n>;

  template<int p, int q, int r>
  constexpr inline
  void mul(return_type& dst) const;

 private:
  const E1& lhs;
  const E2& rhs;

  constexpr MatrixMulMatrix(const E1& lhs, const E2& rhs);
  friend constexpr inline auto operator *<E1, E2, m, l, n>(const MatExpression<E1, m, l>& lhs, const MatExpression<E2, l, n>& rhs);

  template<int p, int q, int r>
  constexpr inline
  void mul(return_type& dst, tag<p, q, r>) const;

  constexpr inline
  void mul(return_type& dst, tag<1, 1, 1>) const;

  template<int p>
  constexpr inline
  void mul(return_type& dst, tag<1, p, 1>) const;

  // TODO: optimize 2n^2n matrix multiplication
//  constexpr inline
//  void mul(return_type& dst, tag<2, 2, 2>) const;

};

template<typename E1, typename E2, int m, int l, int n>
constexpr static inline auto
operator * (const MatExpression<E1, m, l>& lhs, const MatExpression<E2, l, n>& rhs) {
  using value_type = typename MatrixMulMatrix<E1, E2, m, l, n>::value_type;
  return Matrix<value_type, m, n>(
      matrix_ctor_matmul,
      MatrixMulMatrix<E1, E2, m, l, n>(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs))
  );
}

template<typename E1, typename E2, int m, int l, int n>
constexpr MatrixMulMatrix<E1, E2, m, l, n>::MatrixMulMatrix(const E1& lhs, const E2& rhs) : lhs(lhs), rhs(rhs) {}

template<typename E1, typename E2, int m, int l, int n>
template<int p, int q, int r>
constexpr void MatrixMulMatrix<E1, E2, m, l, n>::mul(MatrixMulMatrix::return_type& dst) const {
  return mul(dst, tag<p, q, r>());
}

template<typename E1, typename E2, int m, int l, int n>
template<int p, int q, int r>
constexpr inline void
MatrixMulMatrix<E1, E2, m, l, n>::mul(return_type& dst, MatrixMulMatrix<E1, E2, m, l, n>::tag<p, q, r>) const {
  using T = typename MatrixMulMatrix<E1, E2, m, l, n>::value_type;

  for( int i = 0; i < m; i++ ) {
    for (int j = 0; j < n; j++) {
      T s = 0;
      for (int k = 0; k < l; k++)
        s += lhs(i, k) * rhs(k, j);
      dst.data[i * n + j] = s;
    }
  }
}

template<typename E1, typename E2, int m, int l, int n>
constexpr inline void
MatrixMulMatrix<E1, E2, m, l, n>::mul(return_type& dst, MatrixMulMatrix::tag<1, 1, 1>) const {
  dst.data[0] = lhs[0] * rhs[0];
}

template<typename E1, typename E2, int m, int l, int n>
template<int p>
constexpr inline void
MatrixMulMatrix<E1, E2, m, l, n>::mul(return_type& dst, MatrixMulMatrix::tag<1, p, 1>) const {
  using T = MatrixMulMatrix<E1, E2, m, l, n>::value_type;

  T sum = 0;
  for(int i=0; i<p; ++i)
    sum += lhs[i] * rhs[i];
  dst.data[0] = sum;
}

//template<typename E1, typename E2, int m, int l, int n>
//constexpr inline void
//MatrixMulMatrix<E1, E2, m, l, n>::mul(return_type& dst, MatrixMulMatrix::tag<2, 2, 2>) const {
//
//}

}

# endif //VCCC_MATH_ALGEBRA_MATRIX_MATRIX_MUL_MATRIX_HPP
