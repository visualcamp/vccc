//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_ITERATOR_INPUT_OR_OUTPUT_ITERATOR_HPP_
#define VCCC_ITERATOR_INPUT_OR_OUTPUT_ITERATOR_HPP_

#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/iterator/weakly_incrementable.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace concepts {

/// @addtogroup iterator
/// @{


/**
@brief
specifies that objects of a type can be incremented and dereferenced

@code{.cpp}
template<typename I>
struct input_or_output_iterator
    : conjunction<
        dereferenceable<I>,
        weakly_incrementable<I>
      > {};
@endcode

The `%input_or_output_iterator` concept forms the basis of the iterator concept taxonomy;
every iterator type satisfies the `%input_or_output_iterator` requirements.

@sa [std::input_or_output_iterator](https://en.cppreference.com/w/cpp/iterator/input_or_output_iterator)


 */
template<typename I>
struct input_or_output_iterator
    : conjunction<
        dereferenceable<I>,
        weakly_incrementable<I>
      > {};

/// @}

} // namespace concepts
} // namespace vccc

#endif // VCCC_ITERATOR_INPUT_OR_OUTPUT_ITERATOR_HPP_
