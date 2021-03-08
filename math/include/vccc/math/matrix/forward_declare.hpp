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

enum flag {

};

static constexpr int flag_default = 0x0;

static constexpr int flag_helper = 0x1;
static constexpr int floag_non_helper = flag_default;


static constexpr int flag_non_alias_safe = 0x2;

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
