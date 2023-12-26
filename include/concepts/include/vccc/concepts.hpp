//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_HPP_
#define VCCC_CONCEPTS_HPP_

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/boolean_testable.hpp"
#include "vccc/concepts/common_reference_with.hpp"
#include "vccc/concepts/common_with.hpp"
#include "vccc/concepts/comparison_common_type_with.hpp"
#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/concepts/copyable.hpp"
#include "vccc/concepts/default_initializable.hpp"
#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/concepts/derived_from.hpp"
#include "vccc/concepts/equality_comparable.hpp"
#include "vccc/concepts/equivalence_relation.hpp"
#include "vccc/concepts/floating_point.hpp"
#include "vccc/concepts/implicit_expression_check.hpp"
#include "vccc/concepts/invocable.hpp"
#include "vccc/concepts/destructible.hpp"
#include "vccc/concepts/movable.hpp"
#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/partially_ordered_with.hpp"
#include "vccc/concepts/predicate.hpp"
#include "vccc/concepts/regular.hpp"
#include "vccc/concepts/relation.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/concepts/semiregular.hpp"
#include "vccc/concepts/signed_integral.hpp"
#include "vccc/concepts/subtractable.hpp"
#include "vccc/concepts/swappable.hpp"
#include "vccc/concepts/totally_ordered.hpp"
#include "vccc/concepts/unsigned_integral.hpp"
#include "vccc/concepts/weakly_equality_comparable_with.hpp"


/**
@defgroup concepts concepts
@brief Concepts

Implementation of concepts is impossible without language features of C++20.
`struct` based implementations (type_traits like) is conducted instead.

Note: Subsume is impossible without using actual concept.

*/

#endif // VCCC_CONCEPTS_HPP_
