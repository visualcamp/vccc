//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_
#define VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that a type is implicitly convertible to another type

@code{.cpp}
template<typename From, typename To>
struct convertible_to :
    conjunction<
      std::is_convertible<From, To>,
      std::is_constructible<To, From>
    > {};
@endcode
See [`std::convertible_to`](https://en.cppreference.com/w/cpp/concepts/convertible_to) for more information
 */
template<typename From, typename To>
struct convertible_to :
    conjunction<
      std::is_convertible<From, To>,
      std::is_constructible<To, From>
    > {};

/// @}



} // namespace vccc

#endif // VCCC_CONCEPTS_CONVERTIBLE_TO_HPP_
