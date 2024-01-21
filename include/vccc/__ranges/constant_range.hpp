//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_RANGES_CONSTANT_RANGE_HPP
#define VCCC_RANGES_CONSTANT_RANGE_HPP

#include <type_traits>

#include "vccc/__concepts/same_as.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iter_const_reference_t.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/view.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<
    typename T,
    bool = conjunction<
               input_iterator<T>,
               has_typename_type<iter_const_reference<T>>
           >::value /* true */
>
struct constant_range_constant_iterator : same_as<iter_const_reference_t<T>, iter_reference_t<T>> {};
template<typename T>
struct constant_range_constant_iterator<T, false> : std::false_type {};

template<typename T, bool = ranges::input_range<T>::value /* true */>
struct constant_range_impl : constant_range_constant_iterator<ranges::iterator_t<T>> {};
template<typename T>
struct constant_range_impl<T, false> : std::false_type {};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief specifies the requirements for a \ref ranges::range "range" to be safely convertible to a \ref ranges::view "view"

The `%constant_range` concept is a refinement of \ref ranges::range "range" for which `ranges::begin` returns a constant
iterator.

@sa [std::ranges::viewable_range](https://en.cppreference.com/w/cpp/ranges/viewable_range)
 */
template<typename T>
struct constant_range : detail::constant_range_impl<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CONSTANT_RANGE_HPP
