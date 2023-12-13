//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_INCREMENTABLE_TRAITS_HPP_
#define VCCC_ITERATOR_INCREMENTABLE_TRAITS_HPP_

#include <cstddef>
#include <type_traits>

#include "vccc/type_traits/empty.hpp"
#include "vccc/type_traits/type_identity.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct has_difference_type : std::false_type {};

template<typename T>
struct has_difference_type<T, void_t<typename T::difference_type>> {};


template<typename T>
struct incrementable_traits_impl;

template<typename T>
struct incrementable_traits_impl<T>
    : std::conditional_t<
        has_difference_type
      > {};

template<typename T>
struct incrementable_traits_impl<T*>
    : std::conditional_t<
        std::is_object<T>::value,
        type_identity<std::ptrdiff_t>,
        empty
      > {};



} // namespace detail

template<typename I>
struct incrementable_traits;

template<typename I>
struct incrementable_traits : detail::incrementable_traits_impl<I> {};

template<typename T>
struct incrementable_traits<const T> : incrementable_traits<T> {};



} // namespace vccc

#endif // VCCC_ITERATOR_INCREMENTABLE_TRAITS_HPP_
