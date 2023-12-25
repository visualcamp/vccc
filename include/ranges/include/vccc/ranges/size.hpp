//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_SIZE_HPP_
#define VCCC_RANGES_SIZE_HPP_

#include <type_traits>

#include "vccc/ranges/decay_copy.hpp"
#include "vccc/ranges/disabled_sized_range.hpp"
#include "vccc/ranges/forward_range.hpp"
#include "vccc/ranges/sentinel_t.hpp"
#include "vccc/type_traits/detail/return_category.hpp"
#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/is_bounded_array.hpp"
#include "vccc/type_traits/is_integer_like.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

template<typename T, bool = is_bounded_array<std::remove_reference_t<T>>::value>
struct size_bounded_array_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct size_bounded_array_check<T, true> : std::true_type {
  using category = return_category<1, std::size_t>;
};

template<typename T, bool = disabled_sized_range<std::remove_cv_t<T>>::value, typename = void>
struct size_member_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct size_member_check<T, false, void_t<decltype( std::declval<T>().size() )>> : is_integer_like<decltype(std::declval<T>().size())> {
  using category = return_category<2, decltype( vccc_decay_copy(std::declval<T>().size()) )>;
};

template<typename T, bool = disabled_sized_range<std::remove_cv_t<T>>::value, typename = void>
struct size_adl_check : std::false_type {
  using category = return_category<0>;
};
template<typename T>
struct size_adl_check<T, false, void_t<decltype( size(std::declval<T>()) )>> : is_integer_like<decltype(size(std::declval<T>()))> {
  using category = return_category<3, decltype( vccc_decay_copy( size(std::declval<T>()) ) )>;
};

template<
    typename T,
    bool = conjunction<
      ranges::forward_range<T>,
      has_typename_type<ranges::sentinel<T>>,
      has_typename_type<ranges::iterator<T>>
    >::value
>
struct size_range_check : std::false_type {
  using category = return_category<0>;
};

template<typename T>
struct size_range_check<T, true>
    : conjunction<
        ranges::forward_range<T>,
        sized_sentinel_for<ranges::sentinel_t<T>, ranges::iterator_t<T>>
      >
{
  using category = return_category<4, std::make_unsigned_t<decltype(ranges::end(std::declval<T>()) - ranges::begin(std::declval<T>()))>>;
};

template<typename T>
struct size_return_category
    : std::conditional_t<
        size_bounded_array_check<T>::value, typename size_bounded_array_check<T>::category,
      std::conditional_t<
        size_member_check<T>::value, typename size_member_check<T>::category,
      std::conditional_t<
        size_adl_check<T>::value, typename size_adl_check<T>::category,
      std::conditional_t<
        size_range_check<T>::value, typename size_range_check<T>::category,
        return_category<0>
      >>>> {};

template<typename T, typename R>
constexpr R size_impl(T&&, return_category<1, R>) {
  return std::extent<std::remove_reference_t<T>>::value;
}

template<typename T, typename R>
constexpr R size_impl(T&& t, return_category<2, R>) {
  return vccc_decay_copy(std::forward<T>(t).size());
}

template<typename T, typename R>
constexpr R size_impl(T&& t, return_category<3, R>) {
  return vccc_decay_copy(size(std::forward<T>(t)));
}

template<typename T, typename R>
constexpr R size_impl(T&& t, return_category<4, R>) {
  return static_cast<R>(ranges::end(std::forward<T>(t)) - ranges::begin(std::forward<T>(t)));
}

} // namespace detail

/// @addtogroup ranges
/// @{

/**
@brief returns the size of a container or array

Calculates the number of elements in `t` in constant time.
 */

template<typename T>
constexpr typename detail::size_return_category<T&&>::return_type
size(T&& t) {
  return detail::size_impl(std::forward<T>(t), detail::size_return_category<T&&>{});
}

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SIZE_HPP_
