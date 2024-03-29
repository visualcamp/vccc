# /*
#  * Created by YongGyu Lee on 2020/02/06.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_MUL_MATRIX_HPP
# define VCCC_MATH_MATRIX_MATRIX_MUL_MATRIX_HPP
#
# include "vccc/__math/matrix/forward_declare.hpp"
# include "vccc/__math/matrix/internal/tag.hpp"

namespace vccc {

namespace internal {
namespace math {

template<typename LhsType, typename RhsType>
struct traits<MatrixMulMatrix<LhsType, RhsType>> {
  enum : int {
    rows = traits<LhsType>::rows,
    cols = traits<RhsType>::cols,
    size = rows * cols,
  };

  enum : int {
    option = traits<LhsType>::option | traits<RhsType>::option | Flag::kAliasUnsafe | Flag::kReferenceUnsafe
  };
  using value_type = typename LhsType::value_type;
};

} // namespace math
} // namespace internal

//! @addtogroup math_matrix
//! @{

template<typename LhsType, typename RhsType>
class MatrixMulMatrix : public MatrixBase<MatrixMulMatrix<LhsType, RhsType>>{
 public:
  using base = MatrixBase<MatrixMulMatrix<LhsType, RhsType>>;
  using value_type  = typename internal::math::traits<MatrixMulMatrix>::value_type;
  using lhs_type = internal::math::hold_type_selector_t<LhsType>;
  using rhs_type = internal::math::hold_type_selector_t<RhsType>;

  constexpr MatrixMulMatrix(const LhsType& lhs, const RhsType& rhs) : lhs(lhs), rhs(rhs) {};

  constexpr auto operator() (std::size_t i) const;
  constexpr auto operator() (std::size_t i, std::size_t j) const;
  constexpr auto operator[] (std::size_t i) const;

 private:
  lhs_type lhs;
  rhs_type rhs;

  using tag = internal::math::tag<int, LhsType::rows, LhsType::cols, RhsType::cols>;

//  template<int p, int q, int r>
//  constexpr
//  void mul(return_type& dst, tag<p, q, r>) const;
//
//  constexpr
//  void mul(return_type& dst, tag<1, 1, 1>) const;
//
//  template<int p>
//  constexpr
//  void mul(return_type& dst, tag<1, p, 1>) const;

  // TODO: optimize 2n^2n matrix multiplication
//  constexpr inline
//  void mul(return_type& dst, tag<2, 2, 2>) const;

};

template<typename E1, typename E2,
    std::enable_if_t<(internal::math::traits<E1>::cols == internal::math::traits<E2>::rows), int> = 0>
constexpr inline
MatrixMulMatrix<E1, E2>
operator*(const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) {
  return MatrixMulMatrix<E1, E2>(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs));
}

//! @cond ignored
template<typename LhsType, typename RhsType>
constexpr auto MatrixMulMatrix<LhsType, RhsType>::operator()(std::size_t i, std::size_t j) const {
  value_type sum(0);
  for(int k=0; k<tag::second; ++k)
    sum += lhs(i, k) * rhs(k, j);
  return sum;
}

template<typename LhsType, typename RhsType>
constexpr auto MatrixMulMatrix<LhsType, RhsType>::operator()(std::size_t i) const {
  return operator()(i / base::cols, i % base::cols);
}

template<typename LhsType, typename RhsType>
constexpr auto MatrixMulMatrix<LhsType, RhsType>::operator[](std::size_t i) const {
  return operator()(i / base::cols, i % base::cols);
}
//! @endcond

//! @} math_matrix


//template<typename E1, typename E2>
//template<int p, int q, int r>
//constexpr void MatrixMulMatrix<E1, E2>::mul(return_type& dst) const {
//  return mul(dst, tag<p, q, r>());
//}
//
//template<typename E1, typename E2>
//template<int p, int q, int r>
//constexpr inline void
//MatrixMulMatrix<E1, E2>::mul(return_type& dst, MatrixMulMatrix<E1, E2>::tag<p, q, r>) const {
//  using T = typename MatrixMulMatrix<E1, E2>::value_type;
//
//  for( int i = 0; i < m; i++ ) {
//    for (int j = 0; j < n; j++) {
//      T s = 0;
//      for (int k = 0; k < l; k++)
//        s += lhs(i, k) * rhs(k, j);
//      dst.data[i * n + j] = s;
//    }
//  }
//}
//
//template<typename E1, typename E2, int m, int l, int n>
//constexpr void
//MatrixMulMatrix<E1, E2, m, l, n>::mul(return_type& dst, MatrixMulMatrix::tag<1, 1, 1>) const {
//  dst.data[0] = lhs[0] * rhs[0];
//}
//
//template<typename LhsType, typename RhsType>>
//template<int p>
//constexpr void
//MatrixMulMatrix<LhsType, RhsType>::mul(MatrixMulMatrix::return_type& dst, MatrixMulMatrix::tag<1, p, 1>) const {
//  using T = MatrixMulMatrix<E1, E2>::value_type;
//
//  T sum = 0;
//  for(int i=0; i<p; ++i)
//    sum += lhs[i] * rhs[i];
//  dst.data[0] = sum;
//}

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_MUL_MATRIX_HPP
