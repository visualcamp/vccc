//
// Created by yonggyulee on 2023/09/29.
//

#ifndef VCCC_TYPE_TRAITS_IS_SWAPPABLE_HPP_
#define VCCC_TYPE_TRAITS_IS_SWAPPABLE_HPP_

#include <utility>
#include <type_traits>

#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T>
using void_t = void;

namespace swappable_test {
using std::swap;

template<typename T, typename U, typename = void>
struct is_swappable_with_impl : std::false_type {};

template<typename T, typename U>
struct is_swappable_with_impl<T, U, void_t<
    decltype((swap(std::declval<T>(), std::declval<U>()),
              swap(std::declval<U>(), std::declval<T>())))>> : std::true_type {};
} // namespace swappable_test

template<typename T, typename U>
using is_swappable_with_impl = swappable_test::is_swappable_with_impl<T, U>;

template<typename T, typename = void>
struct is_referencable : std::false_type {};

template<typename T>
struct is_referencable<T, void_t<T&>> : std::true_type {};

} // namespace detail

template<typename T, typename U>
using is_swappable_with = detail::is_swappable_with_impl<T, U>;

template<typename T>
struct is_swappable :
    std::conditional_t<
        !detail::is_referencable<T>::value,
        std::false_type,
        is_swappable_with<std::add_lvalue_reference_t<T>, std::add_lvalue_reference_t<T>>>{};

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_SWAPPABLE_HPP_
