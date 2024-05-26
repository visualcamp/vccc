//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_CRBEGIN_HPP_
#define VCCC_RANGES_CRBEGIN_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/basic_const_iterator.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/possibly_const_range.hpp"
#include "vccc/__ranges/rbegin.hpp"
#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct crbegin_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T>,
      enable_borrowed_range<std::remove_cv_t<T>>
  >::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    return vccc::const_iterator<decltype(ranges::rbegin(possibly_const_range(t)))>(ranges::rbegin(possibly_const_range(t)));
  }
};

} // namespace detail

namespace niebloid {

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::crbegin_niebloid crbegin{};

/// @}

} // namespace niebloid
using namespace niebloid;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CRBEGIN_HPP_
