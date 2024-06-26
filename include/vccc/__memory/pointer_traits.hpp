//
// Created by YongGyu Lee on 3/7/24.
//

#ifndef VCCC_MEMORY_POINTER_TRAITS_HPP
#define VCCC_MEMORY_POINTER_TRAITS_HPP

#include <cstddef>

#include "vccc/__core/constexpr.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__type_traits/has_typename_difference_type.hpp"
#include "vccc/__type_traits/has_typename_element_type.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename Ptr, bool = has_typename_element_type<Ptr>::value>
struct pointer_traits_element_type {};

template<typename Ptr>
struct pointer_traits_element_type<Ptr, true> {
  using type = typename Ptr::element_type;
};

template<template<typename, typename...> class Ptr, typename T, typename... Args>
struct pointer_traits_element_type<Ptr<T, Args...>, false> {
  using type = T;
};

template<typename Ptr, bool = has_typename_difference_type<Ptr>::value /* true */>
struct pointer_traits_difference_type {
  using type = typename Ptr::difference_type;
};

template<typename Ptr>
struct pointer_traits_difference_type<Ptr, false> {
  using type = std::ptrdiff_t;
};

template<typename T, typename U, typename = void>
struct has_rebind : std::false_type {};
template<typename T, typename U>
struct has_rebind<T, U, void_t<typename T::template rebind<U>>> : std::true_type {};

template<typename Ptr, typename U, bool = has_rebind<Ptr, U>::value>
struct pointer_traits_rebind;

template<typename Ptr, typename U>
struct pointer_traits_rebind<Ptr, U, true> {
  using type = typename Ptr::template rebind<U>;
};

template<template<typename, typename...> class Ptr, typename U, typename T, typename... Args>
struct pointer_traits_rebind<Ptr<T, Args...>, U, false> {
  using type = Ptr<U, Args...>;
};

template<typename Ptr, bool = has_typename_type<pointer_traits_element_type<Ptr>>::value /* true */>
struct pointer_traits_impl {
  using pointer = Ptr;
  using element_type = typename pointer_traits_element_type<Ptr>::type;
  using difference_type = typename pointer_traits_difference_type<Ptr>::type;

  template<typename U>
  using rebind = typename pointer_traits_rebind<Ptr, U>::type;

  static pointer pointer_to(element_type& r) {
    return Ptr::pointer_to(r);
  }
};

template<typename Ptr>
struct pointer_traits_impl<Ptr, false> {};

} // namespace detail

/// @addtogroup memory
/// @{

template<typename Ptr>
struct pointer_traits
    : detail::pointer_traits_impl<Ptr> {};

template<typename T>
struct pointer_traits<T*> {
  using pointer = T*;
  using element_type = T;
  using difference_type = std::ptrdiff_t;

  template<typename U>
  using rebind = U*;

  static VCCC_CONSTEXPR_AFTER_CXX17 pointer pointer_to(element_type& r) noexcept {
    return vccc::addressof(r);
  }
};

/// @}

} // namespace vccc

#endif // VCCC_MEMORY_POINTER_TRAITS_HPP
