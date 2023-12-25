//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_FORWARD_RANGE_HPP_
#define VCCC_RANGES_FORWARD_RANGE_HPP_

#include <type_traits>

#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/ranges/input_range.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = has_typename_type<iterator<T>>::value>
struct forward_range_impl : std::false_type {};

template<typename T>
struct forward_range_impl<T, true>
    : conjunction<
        ranges::input_range<T>,
        forward_iterator<ranges::iterator_t<T>>
      > {};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief specifies a range whose iterator type satisfies `forward_iterator`

The `%forward_range` concept is a refinement of range for which `ranges::begin` returns a model of `forward_iterator`.

@sa [std::ranges::input_range](https://en.cppreference.com/w/cpp/ranges/input_range)
 */

template<typename T>
struct forward_range : detail::input_range_impl<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_FORWARD_RANGE_HPP_
