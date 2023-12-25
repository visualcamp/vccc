//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_INPUT_ITERATOR_HPP_
#define VCCC_ITERATOR_INPUT_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/derived_from.hpp"
#include "vccc/iterator/detail/iter_concept.hpp"
#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename I,
    bool = conjunction<
             input_or_output_iterator<I>,
             indirectly_readable<I>,
             has_typename_type<vccc::detail::ITER_CONCEPT_T<I>>
           >::value
>
struct input_iterator_impl : std::false_type {};

template<typename I>
struct input_iterator_impl<I, true>
    : conjunction<
        input_or_output_iterator<I>,
        indirectly_readable<I>,
        derived_from<vccc::detail::ITER_CONCEPT<I>, input_iterator_tag>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
 *  @brief
 *  specifies that a type is an input iterator, that is, its referenced values can be read and it can be both pre- and post-incremented
 *
 *  The `%input_iterator` concept is a refinement of `input_or_output_iterator`, adding the requirement that the
 *  referenced values can be read (via `indirectly_readable`) and the requirement that the iterator concept tag be present.
 *
 *  <H1>Note</H1>
 *  Unlike the `LegacyInputIterator` requirements, the `%input_iterator` concept does not require `equality_comparable`,
 *  since input iterators are typically compared with sentinels.
 *
 *  @sa [std::input_iterator](https://en.cppreference.com/w/cpp/iterator/input_iterator)
 *  @sa input_or_output_iterator
 */

template<typename I>
struct input_iterator : detail::input_iterator_impl<I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INPUT_ITERATOR_HPP_
