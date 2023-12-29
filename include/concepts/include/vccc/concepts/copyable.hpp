//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_COPYABLE_HPP_
#define VCCC_CONCEPTS_COPYABLE_HPP_

#include <type_traits>

#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/concepts/movable.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = is_referencable<T>::value>
struct copyable_impl
    : conjunction<
        copy_constructible<T>,
        movable<T>,
        assignable_from<T&, T&>,
        assignable_from<T&, const T&>,
        assignable_from<T&, const T>
      > {};

template<typename T>
struct copyable_impl<T, false> : std::false_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of a type can be copied, moved, and swapped

@code{.cpp}
template<typename T, bool = is_referencable<T>::value>
struct copyable_impl
    : conjunction<
        copy_constructible<T>,
        movable<T>,
        assignable_from<T&, T&>,
        assignable_from<T&, const T&>,
        assignable_from<T&, const T>
      > {};
@endcode

The concept `%copyable<T>` specifies that `T` is a movable object type that can also be copied (that is, it supports
copy construction and copy assignment).

@sa [std::copyable](https://en.cppreference.com/w/cpp/concepts/copyable)
@sa movable
 */
template<typename T>
struct copyable : detail::copyable_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COPYABLE_HPP_
