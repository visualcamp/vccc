//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_RANGE_HPP_
#define VCCC_RANGES_RANGE_HPP_

#include <type_traits>

#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = is_referencable<T>::value, typename = void, typename = void>
struct is_range : std::false_type {};

template<typename T>
struct is_range<T, true,
                void_t<decltype(ranges::begin(std::declval<T&>()))>,
                void_t<decltype(ranges::end(std::declval<T&>()))>
                > : std::true_type {};

} // namespace detail

/// @addtogroup ranges
/// @{


/**
@brief
specifies that a type is a range, that is, it provides a begin iterator and an end sentinel

The range concept defines the requirements of a type that allows iteration over its elements by providing an
iterator and sentinel that denote the elements of the range.

<H1>Notes</H1>
A typical range class only needs to provide two functions:

-# A member function `%begin()` whose return type models `input_or_output_iterator`.
-# A member function `%end()` whose return type models `sentinel_for<It>`, where `It` is the return type of `%begin()`.

Alternatively, they can be non-member functions, to be found by argument-dependent lookup.

@sa [std::ranges::range](https://en.cppreference.com/w/cpp/ranges/range)
*/

template<typename T>
struct range : detail::is_range<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_RANGE_HPP_
