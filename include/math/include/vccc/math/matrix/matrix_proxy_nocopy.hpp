# /*
#  * Created by YongGyu Lee on 2020/03/02.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIX_PROXY_NOCOPY_HPP
# define VCCC_MATH_MATRIX_MATRIX_PROXY_NOCOPY_HPP
#
# include "vccc/math/matrix/forward_declare.hpp"
# include "vccc/math/matrix/type_helper.hpp"

namespace vccc {

namespace internal {
namespace math {

template<typename ExprType>
struct traits<MatrixProxyNocopy<ExprType>> {
  enum {
    rows = traits<ExprType>::rows,
    cols = traits<ExprType>::cols,
    size = rows * cols,
  };

  enum {
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

  template<typename E>
  MatrixProxyNocopy& operator = (const MatrixBase<E>& rhs) && {
    MatrixAssigner::assignNocopy(rhs, expr);
    return *this;
  }

 private:
  ExprType& expr;
};

} // namespace vccc

# endif // VCCC_MATH_MATRIX_MATRIX_PROXY_NOCOPY_HPP
