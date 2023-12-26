//
// Created by yonggyulee on 2023/12/26.
//

#ifndef VCCC_UTILITY_CXX20_REL_OPS_HPP_
#define VCCC_UTILITY_CXX20_REL_OPS_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace rel_ops {
namespace detail {

template<typename T, typename U, typename = void>
struct has_operator_equal_2 : std::false_type {};
template<typename T, typename U>
struct has_operator_equal_2<
        T, U,
        void_t<decltype( std::declval<T&>() == std::declval<U&>() )>
    > : std::is_convertible<decltype( std::declval<T&>() == std::declval<U&>() ), bool> {};

template<typename T, typename U, typename = void>
struct has_operator_less_2 : std::false_type {};
template<typename T, typename U>
struct has_operator_less_2<
        T, U,
        void_t<decltype( std::declval<T&>() < std::declval<U&>() )>
    > : std::is_convertible<decltype( std::declval<T&>() < std::declval<U&>() ), bool> {};

} // namespace detail

/// @addtogroup utility
/// @{
/// @addtogroup utility_rel_ops_func__func__rel_ops_operator operator<=>
/// @brief automatically generates comparison operators based on user-defined `operator==` and `operator<` in C++20 way
///
/// Given a user-defined `operator==` and `operator<` for objects of type `T`, implements the usual semantics of other
/// comparison operators.
///
/// Unlike [std::rel_ops::operatorX](https://en.cppreference.com/w/cpp/utility/rel_ops/operator_cmp), this version
/// works even if the types of the both operands are different.
///
/// @{


/// @brief synthesized from `U == T`
template<typename T, typename U, std::enable_if_t<detail::has_operator_equal_2<U, T>::value, int> = 0>
constexpr bool operator==(const T& a, const U& b) {
  return b == a;
}

template<typename T, typename U, typename = void>
struct has_operator_equal : std::false_type {};
template<typename T, typename U>
struct has_operator_equal<
        T, U,
        void_t<decltype( std::declval<T&>() == std::declval<U&>() )>
    > : std::is_convertible<decltype( std::declval<T&>() == std::declval<U&>() ), bool> {};

/// @brief return `!( b < a || a == b)`. synthesized from `U < T` and `T == U`
template<typename T, typename U, std::enable_if_t<conjunction<
    has_operator_equal<T, U>,
    detail::has_operator_less_2<U, T>
  >::value, int> = 0>
constexpr bool operator<(const T& a, const U& b) {
  // (a < b) -> !(a >= b) -> !( a > b || a == b) -> !( b < a || a == b)
  return !( (b < a) || (a == b));
}

template<typename T, typename U, typename = void>
struct has_operator_less : std::false_type {};
template<typename T, typename U>
struct has_operator_less<
        T, U,
        void_t<decltype( std::declval<T&>() < std::declval<U&>() )>
    > : std::is_convertible<decltype( std::declval<T&>() < std::declval<U&>() ), bool> {};

/// @brief return `!(a == b)`. Synthesized from `T == U`
template<typename T, typename U, std::enable_if_t<has_operator_equal<T, U>::value, int> = 0>
constexpr bool operator!=(const T& a, const U& b) {
  return !(a == b);
}

/// @brief return `b < a`. Synthesized from `U < T`
template<typename T, typename U, std::enable_if_t<has_operator_less<U, T>::value, int> = 0>
constexpr bool operator>(const T& a, const U& b) {
  return b < a; // (a > b) -> (b < a)
}

/// @brief return `!(b < a)`. Synthesized from `U < T`
template<typename T, typename U, std::enable_if_t<has_operator_less<U, T>::value, int> = 0>
constexpr bool operator<=(const T& a, const U& b) {
  return !(b < a); // (a <= b) -> !(a > b) -> !(b < a)
}

/// @brief return `!(a < b)`. Synthesized from `T < U`
template<typename T, typename U, std::enable_if_t<has_operator_less<T, U>::value, int> = 0>
constexpr bool operator>=(const T& a, const U& b) {
  return !(a < b); // (a >= b) -> !(a < b)
}

/// @}
/// @}

} // namespace rel_ops
} // namespace vccc

#endif // VCCC_UTILITY_CXX20_REL_OPS_HPP_
