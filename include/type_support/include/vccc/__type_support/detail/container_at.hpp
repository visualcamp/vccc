# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
#
# include <type_traits>
# include <utility>
#
# include "vccc/type_traits.hpp"
# include "vccc/utility.hpp"

namespace vccc {

/**
@addtogroup type_support_at__func
@{
@defgroup type_support_at_container__func vccc::at (container)
Index-based value accessor

@{
*/
template<std::size_t i, typename Container, std::enable_if_t<is_range<Container>::value, int> = 0>
decltype(auto)
at(Container& container)
{
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container, std::enable_if_t<is_range<Container>::value, int> = 0>
decltype(auto)
at(const Container& container)
{
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container, std::enable_if_t<is_range<Container>::value, int> = 0>
decltype(auto)
at(Container&& container)
{
  BOUNDS_ASSERT(i, container.size());
  return std::move(*std::next(std::begin(container), i));
}

template<std::size_t i, typename Container, std::enable_if_t<is_range<Container>::value, int> = 0>
decltype(auto)
at(const Container&& container)
{
  BOUNDS_ASSERT(i, container.size());
  return std::move(*std::next(std::begin(container), i));
}

//! @}
//! @} type_support

}

# endif // VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
