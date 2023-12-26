//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_
#define VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_

#include "vccc/concepts/move_constructible.hpp"
#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename T, bool = is_referencable<T>::value>
struct copy_constructible_impl
    : conjunction<
        move_constructible<T>,
        constructible_from<T, T&>, convertible_to<T&, T>,
        constructible_from<T, const T&>, convertible_to<const T&, T>,
        constructible_from<T, const T>, convertible_to<const T, T>
      > {};

template<typename T>
struct copy_constructible_impl<T, false> : std::false_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that an object of a type can be copy constructed and move constructed

@code{.cpp}
template<typename T, bool = is_referencable<T>::value>
struct copy_constructible_impl
    : conjunction<
        move_constructible<T>,
        constructible_from<T, T&>, convertible_to<T&, T>,
        constructible_from<T, const T&>, convertible_to<const T&, T>,
        constructible_from<T, const T>, convertible_to<const T, T>
      > {};
@endcode

The concept `%copy_constructible` is satisfied if `T` is an lvalue reference type, or if it is a `move_constructible`
object type where an object of that type can constructed from a (possibly const) lvalue or const rvalue of that type in
both direct- and copy-initialization contexts with the usual semantics (a copy is constructed with the source unchanged).

@sa [std::copy_constructible](https://en.cppreference.com/w/cpp/concepts/copy_constructible)
 */
template<typename T>
struct copy_constructible : detail::copy_constructible_impl<T> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COPY_CONSTRUCTIBLE_HPP_
