//
// Created by yonggyulee on 2024/01/22.
//

#ifndef VCCC_RANGES_VIEWS_ENUMERATE_HPP
#define VCCC_RANGES_VIEWS_ENUMERATE_HPP

#include <type_traits>
#include <tuple>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/enumerate_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<template<typename...> class Tuple>
class enumerate_adaptor_closure : public range_adaptor_closure<enumerate_adaptor_closure<Tuple>> {
  public:
  enumerate_adaptor_closure() = default;

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const {
    return enumerate_view<all_t<R>, Tuple>(std::forward<R>(r));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::enumerate_adaptor_closure<std::tuple> enumerate{};

/// @brief return std::pair instead of std::tuple
VCCC_INLINE_OR_STATIC constexpr detail::enumerate_adaptor_closure<std::pair> enumerate_pair{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ENUMERATE_HPP
