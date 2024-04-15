//
// Created by YongGyu Lee on 4/9/24.
//

#ifndef VCCC_TUPLE_TUPLE_FOR_EACH_HPP_
#define VCCC_TUPLE_TUPLE_FOR_EACH_HPP_

#include <cstddef>
#include <tuple>
#include <utility>

#include "vccc/__functional/invoke.hpp"
#include "vccc/__utility/in_place.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/copy_cvref.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename Tuple, typename F, std::size_t... I>
constexpr void tuple_for_each_impl(Tuple&& t, F&& f, std::index_sequence<I...>, std::true_type) {
  int dummy[] = {
      ((void)vccc::invoke(f, std::get<I>(std::forward<Tuple>(t)), in_place_index<I>), 0)...
  };
}

template<typename Tuple, typename F, std::size_t... I>
constexpr void tuple_for_each_impl(Tuple&& t, F&& f, std::index_sequence<I...>, std::false_type) {
  int dummy[] = {
      (vccc::invoke(f, std::get<I>(std::forward<Tuple>(t))), 0)...
  };
  (void)dummy;
}

template<typename Tuple>
using tuple_index_sequence = std::make_index_sequence<std::tuple_size<remove_cvref_t<Tuple>>::value>;

template<typename Tuple, typename F, typename Seq>
struct tuple_for_each_invocable_impl;

template<typename Tuple, typename F, std::size_t... I>
struct tuple_for_each_invocable_impl<Tuple, F, std::index_sequence<I...>>
    : conjunction<
        is_invocable<F, decltype(std::get<I>(std::declval<Tuple>()))>...
    > {};

template<typename Tuple, typename F>
struct tuple_for_each_invocable
    : tuple_for_each_invocable_impl<Tuple, F, tuple_index_sequence<Tuple>> {};

template<typename Tuple, typename F, typename Seq>
struct tuple_for_each_in_place_index_invocable_impl;

template<typename Tuple, typename F, std::size_t... I>
struct tuple_for_each_in_place_index_invocable_impl<Tuple, F, std::index_sequence<I...>>
    : conjunction<
        is_invocable<F, decltype(std::get<I>(std::declval<Tuple>())), in_place_index_t<I>>...
> {};

template<typename Tuple, typename F>
struct tuple_for_each_in_place_index_invocable
    : tuple_for_each_in_place_index_invocable_impl<Tuple, F, tuple_index_sequence<Tuple>> {};

} // namespace detail

/// @addtogroup tuple
/// @{

template<typename Tuple, typename F>
constexpr std::enable_if_t<detail::tuple_for_each_invocable<Tuple, F>::value> tuple_for_each(Tuple&& t, F&& f) {
  return vccc::detail::tuple_for_each_impl(
      std::forward<Tuple>(t),
      std::forward<F>(f),
      detail::tuple_index_sequence<Tuple>{},
      detail::tuple_for_each_in_place_index_invocable<Tuple, F>{});
}

/// @}

} // namespace vccc

#endif // VCCC_TUPLE_TUPLE_FOR_EACH_HPP_
