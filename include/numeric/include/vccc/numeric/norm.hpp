# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_NORM_HPP
# define VCCC_NUMERIC_NORM_HPP
#
# include <cmath>
#
# include "vccc/type_traits.hpp"
# include "vccc/numeric/sum.hpp"

namespace vccc {

/**
@addtogroup numeric
@{
@defgroup numeric_norm norm
@brief calculate norm

\f$ {\large \hat{x} = \sqrt{\sum{x_i ^2}} }\f$
@}

@addtogroup numeric_norm
@{
*/

/**
@brief calculate norm on iterator [first, last)

@param first beginning iterator
@param last  ending iterator
@return norm
 */
template<typename InputIterator, std::enable_if_t<is_iterable<InputIterator>::value, int> = 0>
inline auto
norm(InputIterator first, InputIterator last)
{
  return std::sqrt(square_sum(first, last));
}

//! @cond ignored
template<typename T, std::enable_if_t<std::is_arithmetic<T>::value, int> = 0>
inline T norm(T arg) {
  return std::abs(arg);
}
//! @endcond


/**
@brief calculate norm of variadics
@return norm
 */
template<typename Arg, typename ...Args,
    std::enable_if_t<conjunction<std::is_arithmetic<Arg>, std::is_arithmetic<Args...>>::value, int> = 0>
inline auto
norm(Arg arg, Args... args)
{
  return std::sqrt(square_sum(arg, args...));
}

//! @} numeric_norm
//! @} numeric

} // namespace vccc

#endif // VCCC_NUMERIC_NORM_HPP
