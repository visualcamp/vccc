# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_MUL_SCALAR_HPP
# define VCCC_MATH_MATRIX_MATRIX_MUL_SCALAR_HPP
#
# include "vccc/math/matrix/mat_expression.hpp"
# include "vccc/math/matrix/type_helper.hpp"
# include <type_traits>
# include <cassert>

namespace vccc{

namespace internal{ namespace math {

template<typename LhsType, typename RhsType>
struct traits<MatrixMulScalar<LhsType, RhsType>> {
  enum {
    rows = traits<LhsType>::rows,
    cols = traits<LhsType>::cols,
  };

  enum {
    option = traits<LhsType>::option | Flag::kAliasUnsafe
  };
  using value_type = typename LhsType::value_type;
};

}}

template<typename LhsType, typename RhsType>
class MatrixMulScalar : public MatExpression<MatrixMulScalar<LhsType, RhsType>> {
 public:
  using value_type = typename LhsType::value_type;
  using lhs_type = internal::math::hold_type_selector_t<LhsType>;
  using rhs_type = const std::remove_reference_t<RhsType>;

  constexpr MatrixMulScalar(const LhsType& lhs, const RhsType& value) : lhs(lhs), value(value) {
    static_assert(!is_matrix<RhsType>::value, "");
  }

  constexpr inline decltype(auto) operator() (std::size_t i) const { return lhs(i) * value; }
  constexpr inline decltype(auto) operator() (std::size_t i, std::size_t j) const { return lhs(i, j) * value; }
  constexpr inline decltype(auto) operator[] (std::size_t i) const { return lhs[i] * value; }

 private:
  lhs_type lhs;
  rhs_type value;
};

template<typename LhsType, typename RhsType, std::enable_if_t<!is_matrix<RhsType>::value, int> = 0>
constexpr static inline
MatrixMulScalar<LhsType, RhsType>
operator * (const MatExpression<LhsType>& lhs, const RhsType& value) {
  return MatrixMulScalar<LhsType, RhsType>(*static_cast<const LhsType*>(&lhs), value);
}

template<typename LhsType, typename RhsType>
constexpr static inline
MatrixMulScalar<LhsType, RhsType>
operator / (const MatExpression<LhsType>& lhs, const RhsType& value) {
  static_assert(!is_matrix<RhsType>::value, "Matrix cannot divide other matrix.");
  return MatrixMulScalar<LhsType, RhsType>(*static_cast<const LhsType*>(&lhs), RhsType(1) / value);
}


}

# endif //VCCC_MATH_MATRIX_MATRIX_MUL_SCALAR_HPP
