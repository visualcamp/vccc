//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_INPUT_RANGE_HPP_
#define VCCC_RANGES_INPUT_RANGE_HPP_

#include <type_traits>

#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = has_typename_type<ranges::iterator<T>>::value>
struct input_range_impl : std::false_type {};

template<typename T>
struct input_range_impl<T, true>
    : conjunction<
        ranges::range<T>,
        input_iterator<ranges::iterator_t<T>>
      > {};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief specifies a range whose iterator type satisfies `input_iterator`

The `%input_range` concept is a refinement of range for which `ranges::begin` returns a model of `input_iterator`.

@sa [std::ranges::input_range](https://en.cppreference.com/w/cpp/ranges/input_range)
@sa `ranges::range`
 */

template<typename T>
struct input_range : detail::input_range_impl<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_INPUT_RANGE_HPP_
