# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
#
# include <iterator>
# include <type_traits>
#
# include "vccc/type_traits.hpp"
# include "vccc/utility.hpp"
# include "vccc/type_support/detail/traits.h"

namespace vccc{

/**
@addtogroup type_support
@{

@addtogroup type_support_at
@{
 
@defgroup type_support_at_container vccc::at(range)
Index-based value accessor

@addtogroup type_support_at_container
@{
*/
template<std::size_t i, typename Range,
  std::enable_if_t<
    conjunction<
      is_range<std::decay_t<Range>>,
      negation<detail::is_tuple_like<std::decay_t<Range>>>
    >::value,
  int> = 0>
decltype(auto)
at(Range&& r)
{
  BOUNDS_ASSERT(i, r.size());
  using type = std::decay_t<decltype(*std::begin(r))>;
  return std::forward<detail::copy_cvref_t<Range, type>>(*std::next(std::begin(r), i));
}

//! @} type_support_at_container
//! @} type_support_at
//! @} type_support

}

# endif //VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
