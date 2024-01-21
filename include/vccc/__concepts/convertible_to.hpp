//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_
#define VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_

#include <type_traits>

#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_explicitly_convertible.hpp"

namespace vccc {

/**
@addtogroup concepts
@{
@addtogroup concepts_convertible_to__class__Core_language_concepts convertible_to
@{
@brief specifies that a type is implicitly convertible to another type

@code{.cpp}
template<typename From, typename To>
struct convertible_to :
    conjunction<
      std::is_convertible<From, To>,
      std::is_constructible<To, From>
    > {};
@endcode
 */

/// @brief Models [std::convertible_to](https://en.cppreference.com/w/cpp/concepts/convertible_to)
template<typename From, typename To>
struct convertible_to :
    conjunction<
      std::is_convertible<From, To>,
      is_explicitly_convertible<From, To>
    > {};

/// @}
/// @}



} // namespace vccc

#endif // VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_
