//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_WEAKLY_EQUALITY_COMPARABLE_WITH_HPP_
#define VCCC_CONCEPTS_WEAKLY_EQUALITY_COMPARABLE_WITH_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace concepts {
namespace detail {

template<typename T, typename U, typename = void>
struct is_explicitly_equality_comparable : std::false_type {};
template<typename T, typename U>
struct is_explicitly_equality_comparable<T, U, void_t<decltype(std::declval<T>() == std::declval<U>())>>
    : std::is_convertible<decltype(std::declval<T>() == std::declval<U>()), bool> {};

template<typename T, typename U, typename = void>
struct is_explicitly_non_equality_comparable : std::false_type {};
template<typename T, typename U>
struct is_explicitly_non_equality_comparable<T, U, void_t<decltype(std::declval<T>() != std::declval<U>())>>
    : std::is_convertible<decltype(std::declval<T>() != std::declval<U>()), bool> {};

template<
    typename T,
    typename U,
    bool = conjunction<
             is_referencable<std::remove_reference_t<T>>,
             is_referencable<std::remove_reference_t<U>>
           >::value
>
struct weakly_equality_comparable_with_impl : std::false_type {};

template<typename T, typename U>
struct weakly_equality_comparable_with_impl<T, U, true> {};

} // namespace detail

template<typename T, typename U>
struct weakly_equality_comparable_with
    : conjunction<
        detail::is_explicitly_equality_comparable<
          std::add_lvalue_reference_t<const std::remove_reference_t<T>>,
          std::add_lvalue_reference_t<const std::remove_reference_t<U>>>,
        detail::is_explicitly_equality_comparable<
          std::add_lvalue_reference_t<const std::remove_reference_t<U>>,
          std::add_lvalue_reference_t<const std::remove_reference_t<T>>>,
        detail::is_explicitly_non_equality_comparable<
          std::add_lvalue_reference_t<const std::remove_reference_t<T>>,
          std::add_lvalue_reference_t<const std::remove_reference_t<U>>>,
        detail::is_explicitly_non_equality_comparable<
          std::add_lvalue_reference_t<const std::remove_reference_t<U>>,
          std::add_lvalue_reference_t<const std::remove_reference_t<T>>>
      > {};

} // namespace vccc
} // namespace concepts

#endif // VCCC_CONCEPTS_WEAKLY_EQUALITY_COMPARABLE_WITH_HPP_
