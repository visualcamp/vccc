//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_CREND_HPP_
#define VCCC_RANGES_CREND_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/basic_const_iterator.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/possibly_const_range.hpp"
#include "vccc/__ranges/rend.hpp"
#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct crend_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T>,
      enable_borrowed_range<std::remove_cv_t<T>>
  >::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    return vccc::const_sentinel<decltype(ranges::rend(possibly_const_range(t)))>(ranges::rend(possibly_const_range(t)));
  }
};

} // namespace detail

namespace niebloid {

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::crend_niebloid crend{};

/// @}

} // namespace niebloid
using namespace niebloid;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CREND_HPP_
