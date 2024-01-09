//
// Created by cosge on 2023-12-31.
//

#ifndef VCCC_ITERATOR_OUTPUT_ITERATOR_HPP
#define VCCC_ITERATOR_OUTPUT_ITERATOR_HPP

#include <type_traits>

#include "vccc/__iterator/indirectly_writable.hpp"
#include "vccc/__iterator/input_or_output_iterator.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<
    typename I,
    typename T,
    bool = conjunction<
               input_or_output_iterator<I>,
               indirectly_writable<I, T>
           >::value
>
struct output_iterator_impl
    : std::is_assignable<
          decltype(*std::declval<I&>()++),
          decltype(std::forward<T>(std::declval<T&&>()))
      > {};

template<typename I, typename T>
struct output_iterator_impl<I, T, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that a type is an output iterator for a given value type, that is, values of that type can be written
to it and it can be both pre- and post-incremented

The `%output_iterator` concept is a refinement of `input_or_output_iterator`, adding the requirement that it can be used
to write values of type and value category encoded by `T` (via `indirectly_writable`). `equality_comparable` is not
required.

<H1>Note</H1>
Unlike the `LegacyOutputIterator` requirements, the `output_iterator` concept does not require that the iterator
category tag be defined.

Algorithms on output iterators should be single pass.

@sa [std::output_iterator](https://en.cppreference.com/w/cpp/iterator/output_iterator)
@sa input_or_output_iterator
 */

template<typename I, typename T>
struct output_iterator : detail::output_iterator_impl<I, T> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_OUTPUT_ITERATOR_HPP
