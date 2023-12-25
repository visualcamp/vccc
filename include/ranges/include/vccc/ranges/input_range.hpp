//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_INPUT_RANGE_HPP_
#define VCCC_RANGES_INPUT_RANGE_HPP_

#include <type_traits>

#include "vccc/iterator/input_iterator.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/detail/requires_helper.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::require;

template<typename T, bool = require<ranges::detail::iterator<T>>::value>
struct input_range_impl : std::false_type {};

template<typename T>
struct input_range_impl<T, true>
    : conjunction<
        ranges::range<T>,
        input_iterator<ranges::iterator_t<T>>
      > {};

} // namespace detail

template<typename T>
struct input_range : detail::input_range_impl<T> {};


} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_INPUT_RANGE_HPP_
