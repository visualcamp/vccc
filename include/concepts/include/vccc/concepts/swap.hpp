//
// Created by yonggyulee on 2023/12/21.
//

#ifndef VCCC_CONCEPTS_SWAP_HPP
#define VCCC_CONCEPTS_SWAP_HPP

#include <type_traits>
#include <utility>

#include "vccc/core.hpp"
#include "vccc/algorithm/swap_ranges.hpp"
#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/type_traits/detail/tag.hpp"
#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail_ranges_swap {

using namespace vccc::ranges;
using std::swap;

template<typename T>
constexpr void swap(T&, T&) = delete;

template<typename T, typename U>
constexpr auto test_swap(int) -> decltype(swap(std::declval<T>(), std::declval<U>()), std::true_type{});

template<typename T, typename U>
constexpr auto test_swap(...) -> std::false_type;

template<typename T, typename U, typename Dummy = void, std::enable_if_t<std::is_void<Dummy>::value, int> = 0>
constexpr void do_swap(T&& t, U&& u) noexcept(noexcept(swap(std::forward<T>(t), std::forward<U>(u)))) {
  swap(std::forward<T>(t), std::forward<U>(u));
}

template<typename T, typename U, bool = disjunction<
    std::is_class<std::remove_reference_t<T>>, std::is_enum<std::remove_reference_t<T>>,
    std::is_class<std::remove_reference_t<U>>, std::is_enum<std::remove_reference_t<U>> >::value>
struct adl_swap : std::false_type {};

template<typename T, typename U>
struct adl_swap<T, U, true> : decltype(test_swap<T, U>(0)) {};

} // namespace detail_ranges_swap

namespace detail {

using vccc::detail::tag_1;
using vccc::detail::tag_2;
using vccc::detail::tag_3;
using vccc::detail::conditional_tag;

template<typename T, typename U, typename = void>
struct ranges_swap_array : std::false_type {};

template<typename T, typename U>
struct ranges_swap_same : std::false_type {};

template<typename V>
struct ranges_swap_same<V&, V&>
    : conjunction<
        move_constructible<V>,
        assignable_from<V&, V>
      > {};

template<typename T, typename U>
using swap_category_tag = conditional_tag<
    detail_ranges_swap::adl_swap<T, U>,
    ranges_swap_array<T, U>,
    ranges_swap_same<T, U>>;

template<typename T, typename U>
constexpr void swap_impl(T&& t, U&& u, tag_1)
    noexcept(noexcept(detail_ranges_swap::do_swap(std::forward<T>(t), std::forward<U>(u))))
{
  detail_ranges_swap::do_swap(std::forward<T>(t), std::forward<U>(u));
}

template<typename T, typename U, std::size_t N>
constexpr void swap_impl(T(&t)[N], U(&u)[N], tag_2) noexcept(noexcept((void)ranges::swap_ranges(t, u)));

template<typename V>
constexpr void swap_impl(V& t, V& u, tag_3)
    noexcept(std::is_nothrow_move_constructible<V>::value && std::is_nothrow_move_assignable<V>::value)
{
  V temp(std::move(t));
  t = std::move(u);
  u = std::move(temp);
}

struct swap_niebloid {
  template<typename T, typename U>
  constexpr std::enable_if_t<(swap_category_tag<T&&, U&&>::value > 0)>
  operator()(T&& t, U&& u) const {
    swap_impl(std::forward<T>(t), std::forward<U>(u), swap_category_tag<T&&, U&&>{});
  }
};

} // namespace detail

/// Niebloids
inline namespace niebloid {

/// @addtogroup concepts
/// @{


/**
@brief swaps the values of two objects

Exchanges the values referenced by `t` and `u`.

`vccc::ranges::swap` is a \ref niebloid.

@sa [std::ranges::swap](https://en.cppreference.com/w/cpp/utility/ranges/swap)
@sa swappable
@sa swappable_with
 */
VCCC_INLINE_OR_STATIC constexpr detail::swap_niebloid swap{};

/// @}

} // namespace niebloid

namespace detail {

template<typename T, typename U, std::size_t N>
struct ranges_swap_array<T(&)[N], U(&)[N], void_t<decltype(ranges::swap(*std::declval<T(&)[N]>(), *std::declval<U(&)[N]>()))>>
    : bool_constant<
        noexcept( (void) ranges::swap_ranges( std::declval<T(&)[N]>(), std::declval<U(&)[N]>() ) ) ==
        noexcept( ranges::swap( *std::declval<T(&)[N]>(), *std::declval<U(&)[N]>() ) )
      > {};


template<typename T, typename U, std::size_t N>
constexpr void swap_impl(T(&t)[N], U(&u)[N], tag_2) noexcept(noexcept((void)ranges::swap_ranges(t, u))) {
  for (std::size_t i = 0; i < N; ++i) {
    ranges::swap(t + i, u + i);
  }
}

} // namespace detail

} // namespace ranges
} // namespace vccc

#endif // VCCC_CONCEPTS_SWAP_HPP
