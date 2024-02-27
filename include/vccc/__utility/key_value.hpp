//
// Created by YongGyu Lee on 2/22/24.
//

#ifndef VCCC_UTILITY_KEY_VALUE_HPP
#define VCCC_UTILITY_KEY_VALUE_HPP

#include <cstddef>
#include <tuple>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__tuple/tuple_like.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<std::size_t I>
struct element_niebloid {
  template<typename T, std::enable_if_t<conjunction<
      tuple_like<T>,
      internal::tuple_size_greater_than<remove_cvref_t<T>, I>
  >::value, int> = 0>
  constexpr decltype(auto) operator()(T&& t) const noexcept {
    return std::get<I>(std::forward<T>(t));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::element_niebloid<0> key;
VCCC_INLINE_OR_STATIC constexpr detail::element_niebloid<0> first;
VCCC_INLINE_OR_STATIC constexpr detail::element_niebloid<1> value;
VCCC_INLINE_OR_STATIC constexpr detail::element_niebloid<1> second;

template<std::size_t I>
VCCC_INLINE_OR_STATIC constexpr detail::element_niebloid<I> element;

} // namespace vccc


#endif // VCCC_UTILITY_KEY_VALUE_HPP
