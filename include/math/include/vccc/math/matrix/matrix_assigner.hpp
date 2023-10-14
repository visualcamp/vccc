# /*
#  * Created by YongGyu Lee on 2020/03/16.
#  */
#
# ifndef VCCC_MATH_MATRIX_MATRIXASSIGNER_H
# define VCCC_MATH_MATRIX_MATRIXASSIGNER_H
#
# include "vccc/math/matrix/forward_declare.hpp"
# include "vccc/math/matrix/type_helper.hpp"
# include <type_traits>
# include <string>

namespace vccc {

class MatrixAssigner {
 public:
  using alias_safe_t = std::true_type;
  using alias_unsafe_t = std::false_type;

  template<typename ExpressionType>
  static auto create(ExpressionType&& expr) {
    using traits = internal::math::traits<ExpressionType>;
    return Matrix<typename traits::value_type, traits::rows, traits::cols>(std::forward<ExpressionType>(expr));
  }

  template<typename ExprType, typename DstType>
  static inline void assign(const MatrixBase<ExprType>& expr, MatrixBase<DstType>& dst) {
    assignImpl(internal::math::is_alias_safe_t<ExprType>{}, expr, dst.derived());
  }

  template<typename ExprType, typename DstType>
  static inline void assignNocopy(const MatrixBase<ExprType>& expr, MatrixBase<DstType>& dst) {
    assignImpl(alias_safe_t{}, expr, dst.derived());
  }

 private:
  template<typename ExprType, typename DstType>
  static void assignImpl(alias_safe_t, const MatrixBase<ExprType>& expr, DstType& dst) {
    for(int i=0; i<internal::math::traits<ExprType>::size; ++i)
      dst(i) = expr(i);
  }

  template<typename ExprType, typename DstType>
  static void assignImpl(alias_unsafe_t, const MatrixBase<ExprType>& expr, DstType& dst) {
    DstType copy;
    for(int i=0; i<internal::math::traits<ExprType>::size; ++i)
      copy(i) = expr(i);
    for(int i=0; i<internal::math::traits<ExprType>::size; ++i)
      dst(i) = std::move(copy(i));
  }
};

} // namespace vccc

#endif // VCCC_MATH_MATRIX_MATRIXASSIGNER_H
