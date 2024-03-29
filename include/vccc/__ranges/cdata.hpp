//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_CDATA_HPP_
#define VCCC_RANGES_CDATA_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/basic_const_iterator.hpp"
#include "vccc/__ranges/as_const_pointer.hpp"
#include "vccc/__ranges/data.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/possibly_const_range.hpp"
#include "vccc/__ranges/range_const_reference_t.hpp"
#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct cdata_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T>,
      enable_borrowed_range<std::remove_cv_t<T>>
  >::value, int> = 0>
  constexpr std::remove_reference_t<range_const_reference_t<T>>* operator()(T&& t) const {
    return vccc::ranges::as_const_pointer(ranges::data(possibly_const_range(t)));
  }
};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::cdata_niebloid cdata{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CDATA_HPP_
