//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_HPP_
#define VCCC_CONCEPTS_HPP_

#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/concepts/destructible.hpp"
#include "vccc/concepts/movable.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/concepts/subtractable.hpp"


/**
@defgroup concepts concepts
@brief Concepts

Implementation of concepts in below C++20 is impossible since it is
a language features. `struct` based implementation (type_traits like)
are defined under namespace `vccc::concepts`.
Note: Subsume is impossible without using actual concept.

*/

#endif // VCCC_CONCEPTS_HPP_
