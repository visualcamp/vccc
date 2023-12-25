//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_ASSIGNABLE_FROM_HPP_
#define VCCC_CONCEPTS_ASSIGNABLE_FROM_HPP_

#include <type_traits>

#include "vccc/concepts/common_reference_with.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace impl {

template<
    typename LHS,
    typename RHS,
    bool = conjunction<
        std::is_assignable<LHS, std::add_lvalue_reference_t<std::remove_reference_t<RHS>> >,
        std::is_assignable<LHS, std::add_rvalue_reference_t<std::remove_reference_t<RHS>> >
        >::value
    >
struct assignable_from_requires;

template<typename LHS, typename RHS>
struct assignable_from_requires<LHS, RHS, true>
    : conjunction<
        same_as<decltype( std::declval<LHS>() = std::declval< std::add_lvalue_reference_t<std::remove_reference_t<RHS>> >() ), LHS>,
        same_as<decltype( std::declval<LHS>() = std::declval< std::add_rvalue_reference_t<std::remove_reference_t<RHS>> >() ), LHS>
      > {};

template<typename LHS, typename RHS>
struct assignable_from_requires<LHS, RHS, false> : std::false_type {};


template<typename LHS, typename RHS>
struct assignable_from_impl
    : conjunction<
        common_reference_with<
            const std::remove_reference_t<LHS>&,
            const std::remove_reference_t<RHS>&>,
        assignable_from_requires<LHS, RHS>
      > {};
} // namespace impl

/// @addtogroup concepts
/// @{


/**
@brief specifies that a type is assignable from another type

See [`std::assignable_from`](https://en.cppreference.com/w/cpp/concepts/assignable_from) for more information
*/
template<typename LHS, typename RHS>
struct assignable_from
    : std::conditional_t<
        conjunction<
          std::is_lvalue_reference<LHS>,
          is_referencable< std::remove_reference_t<LHS> >,
          is_referencable< std::remove_reference_t<RHS> > >::value,
        impl::assignable_from_impl<LHS, RHS>,
        std::false_type
    > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_ASSIGNABLE_FROM_HPP_
