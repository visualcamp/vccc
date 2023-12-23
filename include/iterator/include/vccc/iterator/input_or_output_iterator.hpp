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

template<typename I>
struct input_or_output_iterator
    : conjunction<
        dereferenceable<I>,
        weakly_incrementable<I>
      > {};

} // namespace concepts
} // namespace vccc

#endif // VCCC_ITERATOR_INPUT_OR_OUTPUT_ITERATOR_HPP_
