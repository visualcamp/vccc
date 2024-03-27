//
// Created by yonggyulee on 2024/01/16.
//

#ifndef VCCC_RANGES_CEND_HPP_
#define VCCC_RANGES_CEND_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/basic_const_iterator.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/possibly_const_range.hpp"
#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct cend_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T>,
      enable_borrowed_range<std::remove_cv_t<T>>
  >::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    return vccc::const_sentinel<decltype(ranges::end(possibly_const_range(t)))>(ranges::end(possibly_const_range(t)));
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{

/**
@brief returns a sentinel indicating the end of a read-only range

Returns a sentinel for the constant iterator indicating the end of a range.

@sa [std::ranges::cend](https://en.cppreference.com/w/cpp/ranges/cend)
 */

VCCC_INLINE_OR_STATIC constexpr detail::cend_niebloid cend{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CEND_HPP_
