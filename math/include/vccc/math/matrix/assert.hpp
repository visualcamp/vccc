# /*
#  * Created by YongGyu Lee on 2020/03/16.
#  */
#
# ifndef VCCC_MATH_MATRIX_ASSERT_H_
# define VCCC_MATH_MATRIX_ASSERT_H_
#
# include "vccc/math/matrix/forward_declare.hpp"
# include <cassert>
#
# define VCCC_MATH_STATIC_ASSERT(cond, msg)   static_assert((cond), msg)
# define VCCC_MATH_RUNTIME_ASSERT(cond, msg)  assert(((void)(msg), (cond)))
#
# define VCCC_MATH_ASSERT_SAME_SIZE(a, b)       \
  VCCC_MATH_STATIC_ASSERT(                      \
    a::rows == b::rows && a::cols == b::cols,   \
    "Matrix size must be same")

# define VCCC_MATH_ASSERT_1D_MATRIX(type) \
  VCCC_MATH_STATIC_ASSERT(                \
    type::rows == 1 || type::cols == 1,   \
    "Matrix is not a 1-D matrix(vector)")

#
# endif //VCCC_MATH_MATRIX_ASSERT_H_
