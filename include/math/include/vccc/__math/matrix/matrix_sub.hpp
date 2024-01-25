# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_SUB_HPP
# define VCCC_MATH_MATRIX_MATRIX_SUB_HPP
#
# include "vccc/__math/matrix/matrix_base.hpp"
# include "vccc/__math/matrix/type_helper.hpp"

namespace vccc {

namespace internal {
namespace math {

template<typename LhsType, typename RhsType>
struct traits<MatrixSub<LhsType, RhsType>> {
  enum : int {
    rows = LhsType::rows,
    cols = RhsType::cols,
    size = rows * cols,
  };

  enum : int {
    option = traits<LhsType>::option | traits<RhsType>::option | Flag::kReferenceUnsafe
  };
  using value_type = typename LhsType::value_type;
};

} // namespace math
} // namespace internal

//! @addtogroup math_matrix
//! @{

template<typename LhsType, typename RhsType>
class MatrixSub : public MatrixBase<MatrixSub<LhsType, RhsType>> {
 public:
  using lhs_type = internal::math::hold_type_selector_t<LhsType>;
  using rhs_type = internal::math::hold_type_selector_t<RhsType>;

  using value_type = typename LhsType::value_type;

  constexpr MatrixSub(const LhsType& lhs, const RhsType& rhs) noexcept : lhs(lhs), rhs(rhs) {}

  constexpr value_type operator() (std::size_t i) const { return lhs(i) - rhs(i); }
  constexpr value_type operator() (std::size_t i, std::size_t j) const { return lhs(i, j) - rhs(i, j); }
  constexpr value_type operator[] (std::size_t i) const { return lhs[i] - rhs[i]; }

 private:
  lhs_type lhs;
  rhs_type rhs;
};

template<typename E1, typename E2, std::enable_if_t<internal::math::is_same_size<E1, E2>::value, int> = 0>
MatrixSub<E1, E2>
operator-(const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) noexcept {
  return MatrixSub<E1, E2>{*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs)};
}

//! @} math_matrix

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_SUB_HPP
