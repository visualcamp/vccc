# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_RANGE_AT_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_RANGE_AT_HPP
#
# include <type_traits>
# include <utility>
#
# include "vccc/ranges.hpp"
# include "vccc/utility.hpp"

namespace vccc {

/**
@addtogroup type_support_at__func
@{
@defgroup type_support_at_container__func vccc::at (container)
Index-based value accessor

@{
*/
template<std::size_t i, typename R, std::enable_if_t<ranges::range<R>::value, int> = 0>
decltype(auto)
at(R&& container)
{
  BOUNDS_ASSERT(i, ranges::distance(container));
  return *ranges::next(ranges::begin(container), i);
}

//! @}
//! @} type_support

}

# endif // VCCC_TYPE_SUPPORT_DETAIL_RANGE_AT_HPP
