//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_DERIVED_FROM_HPP_
#define VCCC_CONCEPTS_DERIVED_FROM_HPP_

#include <type_traits>

#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

/**
@addtogroup concepts
@{
@addtogroup concepts_derived_from__class__Core_language_concepts derived_from
@brief specifies that a type is derived from another type

@code{.cpp}
template<typename Derived, typename Base>
struct derived_from
    : conjunction<
        std::is_base_of<Base, Derived>,
        std::is_convertible<const volatile Derived*, const volatile Base*>
      > {};
@endcode

The concept `%derived_from<Derived, Base>` is satisfied if and only if `Base` is a class type that is either `Derived`
or a public and unambiguous base of `Derived`, ignoring cv-qualifiers.

Note that this behavior is different to `std::is_base_of` when `Base` is a private or protected base of `Derived`.
@{
 */

/// @brief Models [std::derived_from](https://en.cppreference.com/w/cpp/concepts/derived_from)
template<typename Derived, typename Base>
struct derived_from
    : conjunction<
        std::is_base_of<Base, Derived>,
        std::is_convertible<const volatile Derived*, const volatile Base*>
      > {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_DERIVED_FROM_HPP_
