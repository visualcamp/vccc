//
// Created by cosge on 2023-12-27.
//

#ifndef VCCC_CONCEPTS_INVOCABLE_HPP
#define VCCC_CONCEPTS_INVOCABLE_HPP

#include <cstddef>
#include <type_traits>

#include "vccc/__functional/invoke.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename...>
struct type_tuple;

template<typename F, typename Tup, typename = void>
struct explicit_invocable : std::false_type {};

template<typename F, typename... Args>
struct explicit_invocable<
      F, type_tuple<Args...>,
      void_t<decltype(
          vccc::invoke(
              std::declval<decltype( std::forward<F>   (std::declval<F&&>   ()) )>(),
              std::declval<decltype( std::forward<Args>(std::declval<Args&&>()) )>()...
          )
      )>
    > : std::true_type {};

template<typename F, typename ArgsTuple>
struct invocable_impl : std::false_type {};

template<typename F, typename... Args>
struct invocable_impl<F, type_tuple<Args...>> : explicit_invocable<F, type_tuple<Args...>> {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that a callable type can be invoked with a given set of argument types

The invocable concept specifies that a callable type `F` can be called with a set of arguments `Args...` using the
function template `vccc::invoke`.

@sa [std::invocable](https://en.cppreference.com/w/cpp/concepts/invocable)
 */
template<typename F, typename... Args>
struct invocable : detail::invocable_impl<F, detail::type_tuple<Args...>> {};

/**
@brief \copybrief invocable

The `%regular_invocable` concept adds to the invocable concept by requiring the invoke expression to be
[equality-preserving](https://en.cppreference.com/w/cpp/concepts#Equality_preservation) and not modify either the
function object or the arguments.

@sa [std::regular_invocable](https://en.cppreference.com/w/cpp/concepts/invocable)
 */
template<typename F, typename... Args >
struct regular_invocable : invocable<F, Args...> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_INVOCABLE_HPP
