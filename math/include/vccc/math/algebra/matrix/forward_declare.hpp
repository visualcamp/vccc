# /*
#  * Created by YongGyu Lee on 2020/02/05.
#  */
#
# ifndef VCCC_MATH_ALGEBRA_MATRIX_FORWARD_DECLARE_HPP
# define VCCC_MATH_ALGEBRA_MATRIX_FORWARD_DECLARE_HPP

namespace vccc{

// Base expression class
template<typename T, int m, int n> class MatExpression;

// Actual Matrix class
template <typename T, int m, int n> class Matrix;

// Matrix substitution helper class
struct matrix_sub_normal_t {};
struct matrix_sub_unary_t {};
template<typename E1, typename E2, typename Category, int m, int n> class MatrixSub;

// Matrix addition helper class
template<typename E1, typename E2, int m, int n> class MatrixSum;

// Matrix multiplication helper class
template<typename E, typename T, int m, int n> class MatrixMulScalar;

// out of class Matrix operators
template<typename E1, typename E2, int m, int n>
constexpr static bool
operator == (const MatExpression<E1, m, n>& lhs, const MatExpression<E2, m, n>& rhs);

template<typename E1, typename E2, int m1, int n1, int m2, int n2>
constexpr static bool
operator == (const MatExpression<E1, m1, n1>& lhs, const MatExpression<E2, m2, n2>& rhs);

template<typename E1, typename E2, int m1, int n1, int m2, int n2>
constexpr static bool
operator != (const MatExpression<E1, m1, n1>& lhs, const MatExpression<E1, m2, n2>& rhs);

}

# endif //VCCC_MATH_ALGEBRA_MATRIX_FORWARD_DECLARE_HPP
