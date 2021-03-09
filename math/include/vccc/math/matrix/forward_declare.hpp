# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_MATRIX_FORWARD_DECLARE_HPP
# define VCCC_MATH_MATRIX_FORWARD_DECLARE_HPP

namespace vccc{

namespace internal{
namespace math{

template<typename T> struct traits;
template<typename T> struct traits<const T> : traits<T> {};

enum Flag {
  kDefault = 0b1,
  kAliasUnsafe = 0b10,
  kReferenceUnsafe = 0b100
};

} // namespace math
} // namespace internal

// Base expression class
template<typename Derived> class MatExpression;

// Actual Matrix class
template <typename T, int m, int n> class Matrix;

template<typename E1, typename E2> class MatrixSum;

template<typename E> class MatrixMinus;

template<typename Lhs, typename Rhs> class MatrixSub;

template<typename LhsType, typename RhsType> class MatrixMulScalar;
template<typename LhsType, typename RhsType> class MatrixMulMatrix;

}

# endif //VCCC_MATH_MATRIX_FORWARD_DECLARE_HPP
