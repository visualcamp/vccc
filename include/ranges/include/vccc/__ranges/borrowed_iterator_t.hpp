//
// Created by yonggyulee on 2024/01/08.
//

#ifndef VCCC_RANGES_BORROWED_ITERATOR_T_HPP_
#define VCCC_RANGES_BORROWED_ITERATOR_T_HPP_

#include <type_traits>

#include "vccc/__ranges/borrowed_range.hpp"
#include "vccc/__ranges/dangling.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/range.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool v = range<R>::value /* true */>
struct borrowed_iterator_impl {
  using type = std::conditional_t<borrowed_range<R>::value, ranges::iterator_t<R>, ranges::dangling>;
};

template<typename R>
struct borrowed_iterator_impl<R, false> {};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename R>
struct borrowed_iterator : detail::borrowed_iterator_impl<R> {};

template<typename R>
using borrowed_iterator_t = typename borrowed_iterator<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_BORROWED_ITERATOR_T_HPP_
