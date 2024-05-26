//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_CBEGIN_HPP_
#define VCCC_RANGES_CBEGIN_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/basic_const_iterator.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/possibly_const_range.hpp"
#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct cbegin_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T>,
      enable_borrowed_range<std::remove_cv_t<T>>
  >::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    return vccc::const_iterator<decltype(ranges::begin(vccc::ranges::possibly_const_range(t)))>(ranges::begin(vccc::ranges::possibly_const_range(t)));
  }
};

} // namespace detail

namespace niebloid {

/// @addtogroup ranges
/// @{

/**
@brief returns an iterator to the beginning of a read-only range

Returns an iterator to the first element of the argument.

@sa [std::ranges::cbegin](https://en.cppreference.com/w/cpp/ranges/cbegin)
 */

VCCC_INLINE_OR_STATIC constexpr detail::cbegin_niebloid cbegin{};

/// @}

} // namespace niebloid
using namespace niebloid;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CBEGIN_HPP_
