# /*
#  * Created by YongGyu Lee on 2020/02/24.
#  */
#
# ifndef VCCC_MATH_MATRIX_STATIC_ASSERT_HPP_
# define VCCC_MATH_MATRIX_STATIC_ASSERT_HPP_
#



# define VCCC_MATH_STATIC_ASSERT_MATRIX_SAME_SIZE(a, b)     \
    static_assert(a::rows == b::rows && a::cols == b::cols, \
                  "Matrix size must be same")


#
# endif //VCCC_MATH_MATRIX_STATIC_ASSERT_HPP_
