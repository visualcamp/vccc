//
// Created by yonggyulee on 2024/01/16.
//

#ifndef VCCC_RANGES_CEND_HPP_
#define VCCC_RANGES_CEND_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/basic_const_iterator.hpp"
#include "vccc/__ranges/constant_range.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__type_traits/detail/tag.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__utility/as_const.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct cend_niebloid {
  template<typename T, std::enable_if_t<disjunction<
      std::is_lvalue_reference<T>,
      enable_borrowed_range<std::remove_cv_t<T>>
  >::value, int> = 0>
  constexpr auto operator()(T&& t) const {
    using tag = conditional_tag<constant_range<T>, constant_range<const T>>;
    return this->call(std::forward<T>(t), tag{});
  }

 private:
  template<typename T>
  constexpr auto call(T&& t, vccc::detail::tag_1) const {
    return ranges::end(t);
  }
  template<typename T>
  constexpr auto call(T&& t, vccc::detail::tag_2) const {
    return ranges::end(vccc::as_const(t));
  }
  template<typename T>
  constexpr auto call(T&& t, vccc::detail::tag_else) const {
    return make_const_sentinel(ranges::end(t));
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
