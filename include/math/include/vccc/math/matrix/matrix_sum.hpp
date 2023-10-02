# /*
#  * Created by YongGyu Lee on 2020/02/04.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_SUM_HPP
# define VCCC_MATH_MATRIX_MATRIX_SUM_HPP
#
# include "vccc/math/matrix/matrix_base.hpp"
# include "vccc/math/matrix/type_helper.hpp"
# include "vccc/math/matrix/assert.hpp"

namespace vccc {

namespace internal { namespace math {

template<typename LhsType, typename RhsType>
struct traits<MatrixSum<LhsType, RhsType>> {
  enum {
    rows = traits<LhsType>::rows,
    cols = traits<LhsType>::cols,
    size = rows * cols,
  };

  enum {
    option = traits<LhsType>::option | traits<RhsType>::option | Flag::kReferenceUnsafe
  };
  using value_type = typename LhsType::value_type;
};

}} // namespace internal::math

template<typename LhsType, typename RhsType>
class MatrixSum : public MatrixBase<MatrixSum<LhsType, RhsType>> {
 public:

  using lhs_type = internal::math::hold_type_selector_t<LhsType>;
  using rhs_type = internal::math::hold_type_selector_t<RhsType>;

  using value_type = typename LhsType::value_type;

  constexpr MatrixSum(const LhsType& lhs, const RhsType& rhs) : lhs(lhs), rhs(rhs) {};

  constexpr value_type operator() (std::size_t i) const                { return lhs(i)    + rhs(i);    }
  constexpr value_type operator() (std::size_t i, std::size_t j) const { return lhs(i, j) + rhs(i, j); }
  constexpr value_type operator[] (std::size_t i) const                { return lhs[i]    + rhs[i];    }

 private:
  lhs_type lhs;
  rhs_type rhs;
};

template<typename E1, typename E2, std::enable_if_t<internal::math::is_same_size<E1, E2>::value, int> = 0>
constexpr inline
MatrixSum<E1, E2>
operator+(const MatrixBase<E1>& lhs, const MatrixBase<E2>& rhs) {
  return MatrixSum<E1, E2>(*static_cast<const E1*>(&lhs), *static_cast<const E2*>(&rhs));
}

}

# endif //VCCC_MATH_MATRIX_MATRIX_SUM_HPP
