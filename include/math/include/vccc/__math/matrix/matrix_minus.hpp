# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_MINUS_HPP
# define VCCC_MATH_MATRIX_MATRIX_MINUS_HPP
#
# include "vccc/__math/matrix/matrix_base.hpp"
# include "vccc/__math/matrix/type_helper.hpp"

namespace vccc {

namespace internal {
namespace math {

template<typename E>
struct traits<MatrixMinus<E>> {
  enum : int {
    rows = traits<E>::rows,
    cols = traits<E>::cols,
    size = rows * cols,
  };

  enum : int {
    option = traits<E>::option | Flag::kReferenceUnsafe
  };
  using value_type = typename E::value_type;
};

} // namespace math
} // namespace internal

//! @addtogroup math_matrix
//! @{

template<typename E>
class MatrixMinus : public MatrixBase<MatrixMinus<E>> {
  const E& e;

 public:
  using lhs_type = internal::math::hold_type_selector_t<E>;
  using value_type = typename E::value_type;

  constexpr explicit MatrixMinus(const E& e) : e(e) {}

  constexpr value_type operator() (std::size_t i) const { return -e(i); }
  constexpr value_type operator() (std::size_t i, std::size_t j) const { return -e(i, j); }
  constexpr value_type operator[] (std::size_t i) const { return -e[i]; }
};

template<typename E>
constexpr inline MatrixMinus<E> operator-(const MatrixBase<E>& lhs) {
  return MatrixMinus<E>(*static_cast<const E*>(&lhs));
}

//! @} math_matrix

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_MINUS_HPP
