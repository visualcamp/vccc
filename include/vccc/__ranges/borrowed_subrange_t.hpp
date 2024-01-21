//
// Created by yonggyulee on 2024/01/08.
//

#ifndef VCCC_RANGES_BORROWED_SUBRANGE_T_HPP_
#define VCCC_RANGES_BORROWED_SUBRANGE_T_HPP_

#include <type_traits>

#include "vccc/__ranges/borrowed_range.hpp"
#include "vccc/__ranges/dangling.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/subrange.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool v = range<R>::value /* true */>
struct borrowed_subrange_impl {
  using type = std::conditional_t<borrowed_range<R>::value, ranges::subrange<ranges::iterator_t<R>>, ranges::dangling>;
};

template<typename R>
struct borrowed_subrange_impl<R, false> {};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename R>
struct borrowed_subrange : detail::borrowed_subrange_impl<R> {};

template<typename R>
using borrowed_subrange_t = typename borrowed_subrange<R>::type;

/// @}

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_BORROWED_SUBRANGE_T_HPP_
