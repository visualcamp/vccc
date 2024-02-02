//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_ASSIGNABLE_FROM_HPP_
#define VCCC_CONCEPTS_ASSIGNABLE_FROM_HPP_

#include <type_traits>

#include "vccc/__concepts/common_reference_with.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244)
#endif

namespace vccc {
namespace impl {

template<
    typename LHS, typename RHS,
    bool = std::is_assignable<LHS, RHS&& >::value /* true */>
struct assignable_from_requires
    : same_as<decltype( std::declval<LHS>() = std::declval<RHS>() ), LHS> {};

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
@addtogroup concepts_assignable_from__class__Core_language_concepts assignable_from
@brief specifies that a type is assignable from another type

The concept `%assignable_from<LHS, RHS>` specifies that an expression of the type and value category specified by `RHS`
can be assigned to an lvalue expression whose type is specified by `LHS`.
@{
*/

/// @brief Models [std::assignable_from](https://en.cppreference.com/w/cpp/concepts/assignable_from)
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
/// @}

} // namespace vccc

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // VCCC_CONCEPTS_ASSIGNABLE_FROM_HPP_
