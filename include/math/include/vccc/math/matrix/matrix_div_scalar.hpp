# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_DIV_SCALAR_HPP
# define VCCC_MATH_MATRIX_MATRIX_DIV_SCALAR_HPP
#
# include "vccc/math/matrix/matrix_base.hpp"
# include "vccc/math/matrix/type_helper.hpp"
# include <type_traits>
# include <cassert>

namespace vccc {

namespace internal {
namespace math {

template<typename LhsType, typename RhsType>
struct traits<MatrixDivScalar<LhsType, RhsType>> {
  enum {
    rows = traits<LhsType>::rows,
    cols = traits<LhsType>::cols,
    size = rows * cols,
  };

  enum {
    option = traits<LhsType>::option | Flag::kReferenceUnsafe
  };
  using value_type = typename LhsType::value_type;
};

} // namespace math
} // namespace internal

//! @addtogroup math_matrix
//! @{

template<typename LhsType, typename RhsType>
class MatrixDivScalar : public MatrixBase<MatrixDivScalar<LhsType, RhsType>> {
 public:
  using lhs_type = internal::math::hold_type_selector_t<LhsType>;
  using rhs_type = const std::remove_reference_t<RhsType>;
  using value_type = typename LhsType::value_type;

  constexpr MatrixDivScalar(const LhsType& lhs, const RhsType& value) : lhs(lhs), value(value) {
    static_assert(!is_matrix<RhsType>::value, "");
  }

  constexpr value_type operator() (std::size_t i) const { return lhs(i) / value; }
  constexpr value_type operator() (std::size_t i, std::size_t j) const { return lhs(i, j) / value; }
  constexpr value_type operator[] (std::size_t i) const { return lhs[i] / value; }

 private:
  lhs_type lhs;
  rhs_type value;
};

template<typename LhsType, typename RhsType, std::enable_if_t<!is_matrix<RhsType>::value, int> = 0>
constexpr inline
MatrixDivScalar<LhsType, RhsType>
operator/(const MatrixBase<LhsType>& lhs, const RhsType& value) {
  return MatrixDivScalar<LhsType, RhsType>(*static_cast<const LhsType*>(&lhs), value);
}

//! @} math_matrix

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_DIV_SCALAR_HPP
