//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_COMMON_WITH_HPP_
#define VCCC_CONCEPTS_COMMON_WITH_HPP_

#include <type_traits>

#include "vccc/__concepts/common_reference_with.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/common_type.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename T,
    typename U,
    bool = conjunction<
               same_as< common_type_t<T, U>, common_type_t<U, T> >,
               has_typename_type<
                   common_reference<std::add_lvalue_reference_t<const T>,
                                    std::add_lvalue_reference_t<const U>> >
           >::value /* false */
>
struct common_with_impl_2 : std::false_type {};

template<typename T, typename U>
struct common_with_impl_2<T, U, true>
    : conjunction<
          std::is_constructible< common_type_t<T, U>, decltype(std::declval<T>()) >,
          std::is_constructible< common_type_t<T, U>, decltype(std::declval<U>()) >,
          common_reference_with<
              std::add_lvalue_reference_t<const T>,
              std::add_lvalue_reference_t<const U>>,
          common_reference_with<
              std::add_lvalue_reference_t<common_type_t<T, U>>,
              common_reference_t<
                  std::add_lvalue_reference_t<const T>,
                  std::add_lvalue_reference_t<const U>
              >
          >
      > {};


template<
    typename T,
    typename U,
    bool = conjunction<has_typename_type< common_type<T, U> >,
                       has_typename_type< common_type<U, T> >
           >::value /* true */
>
struct common_with_impl_1 : common_with_impl_2<T, U> {};

template<typename T, typename U>
struct common_with_impl_1<T, U, false> : std::false_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that two types share a common type

The concept `%common_with<T, U>` specifies that two types `T` and `U` share a common type
(as computed by `vccc::common_type_t`) to which both can be converted.

@sa [std::common_with](https://en.cppreference.com/w/cpp/concepts/common_with)
@sa type_traits_common_type__class__Miscellaneous_transformations \copybrief type_traits_common_type__class__Miscellaneous_transformations
@sa type_traits_common_reference__class__Miscellaneous_transformations \copybrief type_traits_common_reference__class__Miscellaneous_transformations
@sa common_reference_with \copybrief common_reference_with
*/
template<typename T, typename U>
struct common_with : detail::common_with_impl_1<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COMMON_REFERENCE_WITH_HPP_
