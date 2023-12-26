//
// Created by yonggyulee on 2023/12/21.
//

#ifndef VCCC_RANGES_SWAP_HPP_
#define VCCC_RANGES_SWAP_HPP_

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/ranges/swap_ranges.hpp"
#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail_ranges_swap {

// Use std::swap ?
using namespace vccc::ranges;

template<typename T>
static constexpr void swap(T&, T&) = delete;

template<typename T, typename U, typename... R>
static constexpr auto test(R&&...) -> decltype(swap(std::declval<T>(), std::declval<U>()), std::true_type{});

template<typename T, typename U>
static constexpr auto test(...) -> std::false_type;

template<typename T, typename U, typename Dummy = void, std::enable_if_t<std::is_void<Dummy>::value, int> = 0>
static constexpr void do_swap(T&& t, U&& u) noexcept(noexcept(swap(std::forward<T>(t), std::forward<U>(u)))) {
  swap(std::forward<T>(t), std::forward<U>(u));
}

} // namespace detail_ranges_swap

namespace detail {

template<typename T, typename U, typename = void>
struct ranges_swap_array : std::false_type {};

template<typename T, typename U>
struct ranges_swap_same : std::false_type {};

template<typename V>
struct ranges_swap_same<V&, V&> : conjunction<
    move_constructible<V>,
    assignable_from<V&, V>
  > {};

template<typename T, typename U>
struct swap_category
    : std::conditional_t<
        decltype(detail_ranges_swap::test<T, U>(0))::value, std::integral_constant<int, 1>,
      std::conditional_t<
        ranges_swap_array<T, U>::value, std::integral_constant<int, 2>,
      std::conditional_t<
        ranges_swap_same<T, U>::value, std::integral_constant<int, 3>,
        std::integral_constant<int, 0>
      >>> {};

template<typename T, typename U>
constexpr void swap_impl(T&& t, U&& u, std::integral_constant<int, 1>)
    noexcept(noexcept(detail_ranges_swap::do_swap(std::forward<T>(t), std::forward<U>(u))))
{
  detail_ranges_swap::do_swap(std::forward<T>(t), std::forward<U>(u));
}

template<typename T, typename U, std::size_t N>
constexpr void swap_impl(T(&t)[N], U(&u)[N], std::integral_constant<int, 2>)
    noexcept(noexcept((void)ranges::swap_ranges(t, u)))
{
  static_assert(always_false<T>::value, "Not implemented yet");
  (void)ranges::swap_ranges(t, u);
}

template<typename V>
constexpr void swap_impl(V& t, V& u, std::integral_constant<int, 3>)
    noexcept(std::is_nothrow_move_constructible<V>::value && std::is_nothrow_move_assignable<V>::value)
{
  V temp(std::move(t));
  (void)(t = std::move(u));
  (void)(u = std::move(temp));
}

} // namespace detail


template<typename T, typename U>
constexpr std::enable_if_t<(detail::swap_category<T&&, U&&>::value > 0)>
swap(T&& t, U&& u) noexcept(noexcept(detail::swap_impl(std::forward<T>(t), std::forward<U>(u), detail::swap_category<T&&, U&&>{}))) {
  detail::swap_impl(std::forward<T>(t), std::forward<U>(u), detail::swap_category<T&&, U&&>{});
}


namespace detail {

template<typename T, typename U, std::size_t N>
struct ranges_swap_array<T(&)[N], U(&)[N], void_t<decltype(ranges::swap(*std::declval<T(&)[N]>(), *std::declval<U(&)[N]>()))>>
    : bool_constant<
        noexcept( (void) ranges::swap_ranges( std::declval<T(&)[N]>(), std::declval<U(&)[N]>() ) ) ==
        noexcept( ranges::swap( *std::declval<T(&)[N]>(), *std::declval<U(&)[N]>() ) )
      > {};

} // namespace detail

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SWAP_HPP_
