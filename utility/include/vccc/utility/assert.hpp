# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_UTILITY_ASSERT_HPP
# define VCCC_UTILITY_ASSERT_HPP
#
# include <cassert>

/** replace to gsl... */

//! @cond ignored

# define ASSERT_IMPL(expr, msg) assert(((void)(msg), (expr)))

//! @endcond

//! @addtogroup utility_assert
//! @{


# define EXPECTS(expr, msg) ASSERT_IMPL(expr, msg)
# define ENSURES(expr, msg) ASSERT_IMPL(expr, msg)

# define BOUNDS_ASSERT(index, size) EXPECTS((index) < (size), "index out of bounds")

# define ALWAYS_ASSERT(msg) ASSERT_IMPL(false, msg)

//! @} utility_assert

# endif //VCCC_UTILITY_ASSERT_HPP
