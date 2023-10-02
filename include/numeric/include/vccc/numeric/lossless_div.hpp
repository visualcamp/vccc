# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_NUMERIC_LOSSLESS_DIV_HPP
# define VCCC_NUMERIC_LOSSLESS_DIV_HPP
#
# include "vccc/type_traits.hpp"

namespace vccc{

//! @addtogroup numeric
//! @{

/**
@brief divide without narrowing

@param a        numerator
@param b        denominator
@return a / b
 */
template<typename T1, typename T2>
decltype(auto)
lossless_div(const T1 a, const T2 b)
{
  using t = lossless_type_div<T1, T2>;
  return static_cast<t>(a) / static_cast<t>(b);
}

//! @} numeric

}

# endif //VCCC_NUMERIC_LOSSLESS_DIV_HPP
