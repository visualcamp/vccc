//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_CONCEPTS_MOVABLE_HPP_
#define VCCC_CONCEPTS_MOVABLE_HPP_

#include <type_traits>

#include "vccc/__concepts/assignable_from.hpp"
#include "vccc/__concepts/move_constructible.hpp"
#include "vccc/__concepts/swappable.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_swappable.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = is_referencable<T>::value /* true */>
struct movable_impl
    : conjunction<
        std::is_object<T>,
        move_constructible<T>,
        assignable_from<T&, T>,
        swappable<T>
      > {};

template<typename T>
struct movable_impl<T, false> : std::false_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of a type can be moved and swapped

@code{.cpp}
template<typename T, bool = is_referencable<T>::value>
struct movable_impl
    : conjunction<
        std::is_object<T>,
        move_constructible<T>,
        assignable_from<T&, T>,
        swappable<T>
      > {};
@endcode

The concept `%movable<T>` specifies that `T` is an object type that can be moved (that is, it can be move constructed,
move assigned, and lvalues of type `T` can be swapped).

@sa [std::movable](https://en.cppreference.com/w/cpp/concepts/movable)
@sa copyable
 */
template<typename T>
struct movable : detail::movable_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_MOVABLE_HPP_
