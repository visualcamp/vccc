//
// Created by yonggyulee on 1/31/24.
//

#ifndef VCCC_VARIANT_VISIT_HPP
#define VCCC_VARIANT_VISIT_HPP

#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__variant/variant.hpp"
#include "vccc/__variant/variant_alternative.hpp"

namespace vccc {
namespace detail {

template<std::size_t Cursor, typename R>
struct visitor_global {
  template<typename T, std::size_t J, typename Vis, std::size_t... I, typename... Variants>
  constexpr R operator()(T&&, in_place_index_t<J>, Vis&& vis, std::index_sequence<I...>, Variants&&... vars) const {
    return visitor_global<Cursor - 1, R>{}.visit(
        std::forward<Vis>(vis), std::index_sequence<I..., J>{}, std::forward<Variants>(vars)...);
  }

  template<typename U, typename Vis, std::size_t... I, typename... Variants>
  constexpr R operator()(U&&, in_place_index_t<variant_npos>, Vis&& vis, std::index_sequence<I...>, Variants&&... vars) const {
    throw bad_variant_access{};
    return visitor_global<Cursor - 1, R>{}.visit(
        std::forward<Vis>(vis), std::index_sequence<I..., 0>{}, std::forward<Variants>(vars)...);
  }

  template<typename Vis, std::size_t...I, typename... Variants>
  constexpr R visit(Vis&& vis, std::index_sequence<I...>, Variants&&... vars) const {
    auto&& var = std::get<Cursor - 1>(std::forward_as_tuple(std::forward<Variants>(vars)...));
    return variant_raw_visit(
        var.index(), std::forward<decltype(var._base().union_)>(var._base().union_), *this,
        std::forward<Vis>(vis), std::index_sequence<I...>{}, std::forward<Variants>(vars)...);
  }
};

template<typename R>
struct visitor_global<0, R> {
  template<typename Vis, std::size_t...I, typename... Variants>
  constexpr R visit(Vis&& vis, std::index_sequence<I...>,  Variants&&... vars) const {
    return vccc::invoke_r<R>(
        std::forward<Vis>(vis),
        std::forward<decltype(variant_raw_get(vars._base().union_, in_place_index<I>))>(
            variant_raw_get(vars._base().union_, in_place_index<I>))...
    );
  }
};

struct as_variant_niebloid {
  template<typename... Ts>
  constexpr variant<Ts...>& operator()(variant<Ts...>& var) const noexcept { return var; }

  template<typename... Ts>
  constexpr const variant<Ts...>& operator()(const variant<Ts...>& var) const noexcept { return var;}

  template<typename... Ts>
  constexpr variant<Ts...>&& operator()(variant<Ts...>&& var) const noexcept { return std::move(var); }

  template<typename... Ts>
  constexpr const variant<Ts...>&& operator()(const variant<Ts...>&& var) const noexcept { return std::move(var); }
};

VCCC_INLINE_OR_STATIC constexpr as_variant_niebloid as_variant{};

template<typename Variant>
using as_variant_t = invoke_result_t<as_variant_niebloid, Variant>;

template<typename Visitor, typename... Variants>
struct variant_visit_result {
  using type = invoke_result_t<Visitor, decltype(get<0>(std::declval<Variants>()))...>;
};

template<typename Visitor, typename... Variants>
using variant_visit_result_t = typename variant_visit_result<Visitor, as_variant_t<Variants>...>::type;

} // namespace detail

/// @addtogroup variant
/// @{

template<typename R, typename Visitor, typename... Variants, std::enable_if_t<conjunction<
    is_invocable<detail::as_variant_niebloid, Variants>...
>::value, int> = 0>
constexpr R visit(Visitor&& vis, Variants&&... vars) {
  return detail::visitor_global<sizeof...(Variants), R>{}.visit(
      std::forward<Visitor>(vis),
      std::index_sequence<>{},
      detail::as_variant(std::forward<Variants>(vars))...
  );
}

template<typename Visitor, typename... Variants, std::enable_if_t<conjunction<
    is_invocable<detail::as_variant_niebloid, Variants>...
>::value, int> = 0>
constexpr decltype(auto)
visit(Visitor&& vis, Variants&&... vars) {
  using R = detail::variant_visit_result_t<Visitor, detail::as_variant_t<Variants>...>;
  return visit<R>(std::forward<Visitor>(vis), std::forward<Variants>(vars)...);
}

/// @}

} // namespace vccc

#endif // VCCC_VARIANT_VISIT_HPP
