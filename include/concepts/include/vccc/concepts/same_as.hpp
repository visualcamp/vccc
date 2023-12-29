//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_SAME_AS_HPP_
#define VCCC_CONCEPTS_SAME_AS_HPP_

#include <type_traits>

namespace vccc {

/**
@addtogroup concepts
@{
@addtogroup concepts_same_as__class__Core_language_concepts same_as
@brief specifies that a type is the same as another type

@code{.cpp}
template<typename T, typename U>
struct same_as : std::is_same<T, U> {};
@endcode

The concept `%same_as<T, U>` is satisfied if and only if `T` and `U` denote the same type.

<H1>Notes</H1>
Implementing [partial ordering](https://en.cppreference.com/w/cpp/language/constraints#Partial_ordering_of_constraints)
is impossible without using actual [concepts](https://en.cppreference.com/w/cpp/language/constraints)
@{
*/

/// @brief Models [std::same_as](https://en.cppreference.com/w/cpp/concepts/same_as)
template<typename T, typename U>
struct same_as : std::is_same<T, U> {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_SAME_AS_HPP_
