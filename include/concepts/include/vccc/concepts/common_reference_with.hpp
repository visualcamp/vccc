//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_
#define VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_

#include <type_traits>

#include "vccc/type_traits/common_reference.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/same_as.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that two types share a common reference type

@code{.cpp}
template<typename T, typename U>
struct common_reference_with
    : conjunction<
        same_as< common_reference_t<T, U>, common_reference_t<U, T> >,
        convertible_to<T, common_reference_t<T, U> >,
        convertible_to<U, common_reference_t<T, U> >
      > {};
@endcode

See [`std::common_reference_with`](https://en.cppreference.com/w/cpp/concepts/common_reference_with) for more information
*/
template<typename T, typename U>
struct common_reference_with
    : conjunction<
        same_as< common_reference_t<T, U>, common_reference_t<U, T> >,
        convertible_to<T, common_reference_t<T, U> >,
        convertible_to<U, common_reference_t<T, U> >
      > {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_
