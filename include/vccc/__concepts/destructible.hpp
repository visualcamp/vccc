//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_DESTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_DESTRUCTIBLE_HPP_

#include <type_traits>

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of the type can be destroyed

@code{.cpp}
template<typename T>
struct destructible : std::is_nothrow_destructible<T> {};
@endcode

The concept `%destructible` specifies the concept of all types whose instances can safely be destroyed at the end of
their lifetime (including reference types).

<H1>Notes</H1>
Unlike the [Destructible](https://en.cppreference.com/w/cpp/named_req/Destructible) named requirement,
`vccc::destructible` requires the destructor to be `noexcept(true)`, not merely non-throwing when invoked,
and allows reference types and array types.

@sa [std::destructible](https://en.cppreference.com/w/cpp/concepts/destructible)
 */
template<typename T>
struct destructible : std::is_nothrow_destructible<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_DESTRUCTIBLE_HPP_
