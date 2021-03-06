# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
# define VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
#
# include "vccc/type_traits.hpp"
# include "vccc/utility.hpp"

namespace vccc{

/**
@addtogroup type_support_at
@{

@defgroup type_support_at_container vccc::at (container)
Index-based value accessor

@addtogroup type_support_at_container
@{
*/
template<std::size_t i, typename Container,
         VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto)
at(Container& container)
{
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container,
         VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto)
at(const Container& container)
{
  BOUNDS_ASSERT(i, container.size());
  return *std::next(std::begin(container), i);
}

template<std::size_t i, typename Container,
         VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto)
at(Container&& container)
{
  BOUNDS_ASSERT(i, container.size());
  return std::move(*std::next(std::begin(container), i));
}

template<std::size_t i, typename Container,
         VCCC_REQUIRE(is_container_v<Container>)>
decltype(auto)
at(const Container&& container)
{
  BOUNDS_ASSERT(i, container.size());
  return std::move(*std::next(std::begin(container), i));
}

//! @} type_support_at_container
//! @} type_support_at

}

# endif //VCCC_TYPE_SUPPORT_DETAIL_CONTAINER_AT_HPP
