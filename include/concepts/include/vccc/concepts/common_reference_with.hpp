//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_
#define VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_

#include <type_traits>

#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/common_reference.hpp"

namespace vccc {
namespace detail {

template<
    typename T,
    typename U,
    bool = conjunction<
               has_typename_type<common_reference<T, U>>,
               has_typename_type<common_reference<T, U>>
           >::value /* false */
>
struct common_reference_with_impl : std::false_type {};

template<typename T, typename U>
struct common_reference_with_impl<T, U, true>
    : conjunction<
        same_as< common_reference_t<T, U>, common_reference_t<U, T> >,
        convertible_to<T, common_reference_t<T, U> >,
        convertible_to<U, common_reference_t<T, U> >
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that two types share a common reference type

The concept `%common_reference_with<T, U>` specifies that two types `T` and `U` share a common reference type
(as computed by `vccc::common_reference_t`) to which both can be converted.

@sa [std::common_reference_with](https://en.cppreference.com/w/cpp/concepts/common_reference_with)
@sa type_traits_common_reference__class__Miscellaneous_transformations \copybrief type_traits_common_reference__class__Miscellaneous_transformations
@sa common_with \copybrief common_with
@sa type_traits_common_type__class__Miscellaneous_transformations \copybrief type_traits_common_type__class__Miscellaneous_transformations
*/
template<typename T, typename U>
struct common_reference_with : detail::common_reference_with_impl<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_
