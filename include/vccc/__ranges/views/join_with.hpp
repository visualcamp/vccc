//
// Created by yonggyulee on 2/13/24.
//

#ifndef VCCC_RANGES_VIEWS_JOIN_WITH_HPP_
#define VCCC_RANGES_VIEWS_JOIN_WITH_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__concepts/derived_from_single_crtp.hpp"
#include "vccc/__concepts/different_from.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/join_with_view.hpp"
#include "vccc/__ranges/views/single.hpp"
#include "vccc/__type_traits/is_implicitly_constructible.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<typename T>
struct first_template_arg;
template<template<typename T, typename...> class Class, typename T1, typename... Ts>
struct first_template_arg<Class<T1, Ts...>> {
  static_assert(derived_from_single_crtp<Class<T1, Ts...>, view_interface>::value, "Class must be view");
  using type = T1;
};

template<typename T>
using first_template_arg_t = typename first_template_arg<T>::type;

// Using its own implementation to support lazy pattern evaluation
template<typename Pattern>
class join_with_adaptor : public range_adaptor_closure<join_with_adaptor<Pattern>> {
  using prv = first_template_arg_t<Pattern>;

 public:
  template<typename U, std::enable_if_t<different_from<join_with_adaptor, U>::value, int> = 0>
  constexpr explicit join_with_adaptor(U&& pattern)
      : pattern_(std::forward<U>(pattern)) {}

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) & {
    return call(*this, std::forward<R>(r), is_implicitly_constructible<range_value_t<range_reference_t<R>>, prv&>{});
  }

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const& {
    return call(*this, std::forward<R>(r), is_implicitly_constructible<range_value_t<range_reference_t<R>>, const prv&>{});
  }

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) && {
    return call(std::move(*this), std::forward<R>(r), is_implicitly_constructible<range_value_t<range_reference_t<R>>, prv&&>{});
  }

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const&& {
    return call(std::move(*this), std::forward<R>(r), is_implicitly_constructible<range_value_t<range_reference_t<R>>, const prv&&>{});
  }

 private:
  constexpr decltype(auto) value_ref() & { return *pattern_.begin(); }
  constexpr decltype(auto) value_ref() const& { return *pattern_.begin(); }
  constexpr decltype(auto) value_ref() && { return std::move(*pattern_.begin()); }
  constexpr decltype(auto) value_ref() const&& { return std::move(*pattern_.begin()); }

  template<typename This, typename R>
  constexpr static auto call(This&& thiz, R&& r, std::true_type) {
    return join_with_view<all_t<R>, single_view<range_value_t<range_reference_t<R>>>>(
        std::forward<R>(r), std::forward<This>(thiz).value_ref());
  }

  template<typename This, typename R>
  constexpr static auto call(This&& thiz, R&& r, std::false_type) {
    return join_with_view<all_t<R>, Pattern>{std::forward<R>(r), std::forward<This>(thiz).pattern_};
  }

  Pattern pattern_;
};

struct join_with_niebloid {
  template<typename R, typename Pattern, std::enable_if_t<conjunction<
      viewable_range<R>,
      has_typename_type<all_t_impl<Pattern>>,
      negation<is_implicitly_constructible<range_value_t<range_reference_t<R>>, Pattern>>
  >::value, int> = 0>
  constexpr auto operator()(R&& r, Pattern&& pattern) const {
    return join_with_view<all_t<R>, all_t<Pattern>>{std::forward<R>(r), std::forward<Pattern>(pattern)};
  }

  template<typename R, std::enable_if_t<conjunction<
      viewable_range<R>
  >::value, int> = 0>
  constexpr auto operator()(R&& r, range_value_t<range_reference_t<R>> pattern) const {
    return join_with_view<all_t<R>, single_view<range_value_t<range_reference_t<R>>>>{
        std::forward<R>(r), std::move(pattern)};
  }

  template<typename Pattern>
  constexpr auto operator()(Pattern&& pattern) const {
    return create_closure(std::forward<Pattern>(pattern), has_typename_type<all_t_impl<Pattern>>{});
  }

 private:
  template<typename Pattern>
  constexpr auto create_closure(Pattern&& pattern, std::true_type /* all */) const {
    return join_with_adaptor<views::all_t<Pattern>>(std::forward<Pattern>(pattern));
  }

  template<typename Pattern>
  constexpr auto create_closure(Pattern&& pattern, std::false_type /* all */) const {
    return join_with_adaptor<decltype(views::single(std::forward<Pattern>(pattern)))>(std::forward<Pattern>(pattern));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::join_with_niebloid join_with{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_JOIN_WITH_HPP_
