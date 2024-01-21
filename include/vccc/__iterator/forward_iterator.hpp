//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_FORWARD_ITERATOR_HPP_
#define VCCC_ITERATOR_FORWARD_ITERATOR_HPP_

#include <type_traits>

#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__iterator/detail/iter_concept.hpp"
#include "vccc/__iterator/incrementable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<typename I, bool = input_iterator<I>::value>
struct forward_iterator_impl : std::false_type {};

template<typename I>
struct forward_iterator_impl<I, true>
    : conjunction<
        derived_from<ITER_CONCEPT<I>, forward_iterator_tag>,
        incrementable<I>,
        sentinel_for<I, I>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief
specifies that an `input_iterator` is a forward iterator, supporting equality comparison and multi-pass

This concept refines `input_iterator` by requiring that `I` also model `incrementable` (thereby making it suitable for
multi-pass algorithms), and guaranteeing that two iterators to the same range can be compared against each other.

<H1>Note</H1>
Unlike the `LegacyForwardIterator` requirements, the `%forward_iterator` concept does not require dereference to return a reference.

@sa [std::forward_iterator](https://en.cppreference.com/w/cpp/iterator/forward_iterator)
@sa input_iterator
 */

template<typename I>
struct forward_iterator : detail::forward_iterator_impl<I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_FORWARD_ITERATOR_HPP_
