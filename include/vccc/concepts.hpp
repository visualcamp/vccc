//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_HPP_
#define VCCC_CONCEPTS_HPP_

#include "vccc/__concepts/assignable_from.hpp"
#include "vccc/__concepts/boolean_testable.hpp"
#include "vccc/__concepts/common_reference_with.hpp"
#include "vccc/__concepts/common_with.hpp"
#include "vccc/__concepts/comparison_common_type_with.hpp"
#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/copyable.hpp"
#include "vccc/__concepts/default_initializable.hpp"
#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__concepts/derived_from_single_crtp.hpp"
#include "vccc/__concepts/different_from.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__concepts/equivalence_relation.hpp"
#include "vccc/__concepts/floating_point.hpp"
#include "vccc/__concepts/implicit_expression_check.hpp"
#include "vccc/__concepts/invocable.hpp"
#include "vccc/__concepts/destructible.hpp"
#include "vccc/__concepts/movable.hpp"
#include "vccc/__concepts/move_constructible.hpp"
#include "vccc/__concepts/partially_ordered_with.hpp"
#include "vccc/__concepts/predicate.hpp"
#include "vccc/__concepts/regular.hpp"
#include "vccc/__concepts/relation.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__concepts/semiregular.hpp"
#include "vccc/__concepts/signed_integral.hpp"
#include "vccc/__concepts/strict_weak_order.hpp"
#include "vccc/__concepts/subtractable.hpp"
#include "vccc/__concepts/swap.hpp"
#include "vccc/__concepts/swappable.hpp"
#include "vccc/__concepts/swappable_with.hpp"
#include "vccc/__concepts/totally_ordered.hpp"
#include "vccc/__concepts/unsigned_integral.hpp"
#include "vccc/__concepts/value_swappable.hpp"
#include "vccc/__concepts/weakly_equality_comparable_with.hpp"


/**
@defgroup concepts concepts
@brief Concepts

Implementation of concepts is impossible without language features of C++20.
`struct` based implementations (type_traits like) is conducted instead.

Note: Subsume is impossible without using actual concept.

*/

#endif // VCCC_CONCEPTS_HPP_
