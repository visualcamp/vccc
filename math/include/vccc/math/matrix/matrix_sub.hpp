# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_SUB_HPP
# define VCCC_MATH_MATRIX_MATRIX_SUB_HPP
#
# include "vccc/math/matrix/mat_expression.hpp"
# include "vccc/math/matrix/type_helper.hpp"
# include "vccc/math/matrix/static_assert.hpp"

namespace vccc {

namespace internal{ namespace math {

template<typename LhsType, typename RhsType>
struct traits<MatrixSub<LhsType, RhsType>> {
  enum {
    rows = LhsType::rows,
    cols = RhsType::cols,
    flags = flag_default | flag_helper
  };
  using value_type = typename LhsType::value_type;
};

}} // namespace internal::math

template<typename LhsType, typename RhsType>
class MatrixSub : public MatExpression<MatrixSub<LhsType, RhsType>> {
 public:
  using lhs_type = internal::math::hold_type_selector_t<LhsType>;
  using rhs_type = internal::math::hold_type_selector_t<RhsType>;

  using value_type = typename LhsType::value_type;

  constexpr inline MatrixSub(const LhsType& lhs, const RhsType& rhs) : lhs(lhs), rhs(rhs) {
    VCCC_MATH_STATIC_ASSERT_MATRIX_SAME_SIZE(LhsType, RhsType);
  }

  constexpr inline decltype(auto) operator() (std::size_t i) const { return lhs(i) - rhs(i); }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const { return lhs(i, j) - rhs(i, j); }
  constexpr inline decltype(auto) operator[] (std::size_t i) const { return lhs[i] - rhs[i]; }

 private:
  lhs_type lhs;
  rhs_type rhs;
};

template<typename E1, typename E2>
MatrixSub<E1, E2>
operator - (const MatExpression<E1>& lhs, const MatExpression<E2>& rhs) {
  return MatrixSub<E1, E2>{*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs)};
}

}



# endif //VCCC_MATH_MATRIX_MATRIX_SUB_HPP
