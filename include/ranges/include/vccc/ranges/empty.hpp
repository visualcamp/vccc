//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_EMPTY_HPP_
#define VCCC_RANGES_EMPTY_HPP_

#include <type_traits>

#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/type_traits/detail/return_category.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

template<typename T, typename = void>
struct empty_member_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct empty_member_check<T, void_t<decltype( bool(std::declval<T>().empty()) )>> : std::true_type {
  using category = return_category<1, bool>;
};

template<typename T, typename = void>
struct empty_size_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct empty_size_check<T, void_t<decltype( ranges::size(std::declval<T>()) == 0 )>> : std::true_type {
  using category = return_category<2, bool>;
};

template<typename T, typename = void>
struct empty_range_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct empty_range_check<
      T,
      void_t<decltype( bool(ranges::begin(std::declval<T>()) == ranges::end(std::declval<T>())) )>
    > : forward_iterator<decltype( ranges::begin(std::declval<T>()) )>
{
  using category = return_category<3, bool>;
};

template<typename T>
struct empty_return_category
    : std::conditional_t<
        empty_member_check<T>::value, typename empty_member_check<T>::category,
      std::conditional_t<
        empty_size_check<T>::value, typename empty_size_check<T>::category,
      std::conditional_t<
        empty_range_check<T>::value, typename empty_range_check<T>::category,
        return_category<0>
      >>> {};

template<typename T, typename R>
constexpr R empty_impl(T&& t, return_category<1, R>) {
  return bool(std::forward<T>(t).empty());
}

template<typename T, typename R>
constexpr R empty_impl(T&& t, return_category<2, R>) {
  return (ranges::size(std::forward<T>(t)) == 0);
}

template<typename T, typename R>
constexpr R empty_impl(T&& t, return_category<3, R>) {
  return bool(ranges::begin(std::forward<T>(t)) == ranges::end(std::forward<T>(t)));
}

} // namespace detail

/// @addtogroup ranges
/// @{

/**
@brief checks whether a range is empty

Determines whether or not `t` has any elements.
 */

template<typename T>
constexpr typename detail::empty_return_category<T&&>::return_type
empty(T&& t) {
  return detail::empty_impl(std::forward<T>(t), detail::empty_return_category<T&&>{});
}

/// @}


} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_EMPTY_HPP_
