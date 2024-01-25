# /*
#  * Created by YongGyu Lee on 2020/03/02.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_PROXY_NOCOPY_HPP
# define VCCC_MATH_MATRIX_MATRIX_PROXY_NOCOPY_HPP
#
# include "vccc/__math/matrix/forward_declare.hpp"
# include "vccc/__math/matrix/type_helper.hpp"

namespace vccc {

namespace internal {
namespace math {

template<typename ExprType>
struct traits<MatrixProxyNocopy<ExprType>> {
  enum : int {
    rows = traits<ExprType>::rows,
    cols = traits<ExprType>::cols,
    size = rows * cols,
  };

  enum : int {
    option = Flag::kDefault | Flag::kReferenceUnsafe
  };
  using value_type = typename traits<ExprType>::value_type;
};

} // namespace math
} // namespace internal

template<typename ExprType>
class MatrixProxyNocopy : public MatrixBase<MatrixProxyNocopy<ExprType>> {
 public:
  static_assert(internal::math::is_concrete_matrix<ExprType>::value, "Only Concrete Matrix type is allowed");

  explicit MatrixProxyNocopy(ExprType& expr) : expr(expr) {}

  template<typename E, std::enable_if_t<internal::math::is_same_size<ExprType, E>::value, int> = 0>
  MatrixProxyNocopy& operator = (const MatrixBase<E>& rhs) && {
    MatrixAssigner::assignNocopy(rhs, expr);
    return *this;
  }

 private:
  ExprType& expr;
};

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_PROXY_NOCOPY_HPP
