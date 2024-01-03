//
// Created by yonggyulee on 2023/12/26.
//

#ifndef VCCC_UTILITY_CXX20_REL_OPS_HPP_
#define VCCC_UTILITY_CXX20_REL_OPS_HPP_

#include <type_traits>

#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/negation.hpp"
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

/**
@addtogroup utility
@{
@addtogroup utility_rel_ops_func__func__rel_ops_operator operator<=>
@brief automatically generates comparison operators based on user-defined `operator==` and `operator<` in C++20 way

Given a user-defined `operator==` and `operator<` for objects of type `T`, implements the usual semantics of other
comparison operators.

Unlike [std::rel_ops::operatorX](https://en.cppreference.com/w/cpp/utility/rel_ops/operator_cmp), this version
works even if the types of the both operands are different.

-# Synthesize `operator==(T, U)` from `operator==(U, T)`
-# Synthesize `operator!=(T, U)` from `operator==(T, U)`
-# Synthesize `operator<(T, U)` from `operator==(T, U)` and `operator<(U, T)`
-# Synthesize `operator>(T, U)` from `operator<(U, T)`
-# Synthesize `operator<=(T, U)` from `operator<(U, T)`
-# Synthesize `operator>=(T, U)` from `operator<(T, U)`

Use the following `constexpr` functions to test if an operator can be found in unqualified context.
- `vccc::rel_ops::is_equality_comparable<T, U>()`
- `vccc::rel_ops::is_non_equality_comparable<T, U>()`
- `vccc::rel_ops::is_less_than_comparable<T, U>()`
- `vccc::rel_ops::is_less_equal_than_comparable<T, U>()`
- `vccc::rel_ops::is_greater_than_comparable<T, U>()`
- `vccc::rel_ops::is_greater_equal_than_comparable<T, U>()`

<H1>Notes</H1>
Since VCCC follows the latest conformance possible, `vccc::rel_ops` is used in every implementation context.

<H1>Example</H1>
```.cpp
#include "vccc/utility.hpp"

struct A {
  int data;
  bool operator==(int other) const { return data == other; }
  bool operator<(int other) const { return data < other; }
};

int main() {
  int x;
  A y;

  y == x; // valid expression: A::operator==(int)
  y < x; // valid expression: A::operator<(int)

  // Following expressions are illegal until C++20
  // x == y;
  // y != x;
  // x != y;

  // y <= x;
  // y > x;
  // y >= x;
  // x < y;
  // x > y;
  // x <= y;
  // x >= y;

  {
    using namespace vccc::rel_ops;

    x == y; // synthesized from y == x
    y != x; // synthesized from !(y == x)
    x != y; // synthesized from !(x == y)

    y <= x;
    y > x;
    y >= x;

    x < y;
    x > y;
    x <= y;
    x >= y;
  }

  // Test if above expressions can be found in unqualified name lookup
  static_assert(vccc::rel_ops::is_equality_comparable<int&, A&>(), "");
  static_assert(vccc::rel_ops::is_non_equality_comparable<int&, A&>(), "");
  static_assert(vccc::rel_ops::is_less_than_comparable<int&, A&>(), "");
  static_assert(vccc::rel_ops::is_less_equal_than_comparable<int&, A&>(), "");
  static_assert(vccc::rel_ops::is_greater_than_comparable<int&, A&>(), "");
  static_assert(vccc::rel_ops::is_greater_equal_than_comparable<int&, A&>(), "");

  return 0;
}
```
@{
*/

/// @brief synthesized from `U == T`
template<typename T, typename U, std::enable_if_t<conjunction<
    detail::has_operator_equal_2<U, T>,
    negation<std::is_same<T, U>>
>::value, int> = 0>
constexpr bool operator==(const T& a, const U& b) {
  return b == a;
}

template<typename T, typename U, typename = void>
struct has_operator_equal : std::false_type {};
template<typename T, typename U>
struct has_operator_equal<
        T, U,
        void_t<decltype( std::declval<T>() == std::declval<U>() )>
    > : std::is_convertible<decltype( std::declval<T>() == std::declval<U>() ), bool> {};

/// @brief return `!( b < a || a == b)`. synthesized from `U < T` and `T == U`
template<typename T, typename U, std::enable_if_t<conjunction<
    has_operator_equal<T, U>,
    detail::has_operator_less_2<U, T>,
    negation<std::is_same<T, U>>
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
        void_t<decltype( std::declval<T>() < std::declval<U>() )>
    > : std::is_convertible<decltype( std::declval<T>() < std::declval<U>() ), bool> {};

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



template<typename T, typename U, typename = void>
struct has_operator_not_equal : std::false_type {};
template<typename T, typename U>
struct has_operator_not_equal<
        T, U,
        void_t<decltype( std::declval<T>() != std::declval<U>() )>
    > : std::is_convertible<decltype( std::declval<T>() != std::declval<U>() ), bool> {};

template<typename T, typename U, typename = void>
struct has_operator_less_equal : std::false_type {};
template<typename T, typename U>
struct has_operator_less_equal<
        T, U,
        void_t<decltype( std::declval<T>() <= std::declval<U>() )>
    > : std::is_convertible<decltype( std::declval<T>() <= std::declval<U>() ), bool> {};

template<typename T, typename U, typename = void>
struct has_operator_greater : std::false_type {};
template<typename T, typename U>
struct has_operator_greater<
        T, U,
        void_t<decltype( std::declval<T>() > std::declval<U>() )>
    > : std::is_convertible<decltype( std::declval<T>() > std::declval<U>() ), bool> {};

template<typename T, typename U, typename = void>
struct has_operator_greater_equal : std::false_type {};
template<typename T, typename U>
struct has_operator_greater_equal<
        T, U,
        void_t<decltype( std::declval<T>() >= std::declval<U>() )>
    > : std::is_convertible<decltype( std::declval<T>() >= std::declval<U>() ), bool> {};


// Use unqualified name lookup

template<typename T, typename U> constexpr bool is_equality_comparable()           noexcept { return has_operator_equal        <T, U>::value; }
template<typename T, typename U> constexpr bool is_non_equality_comparable()       noexcept { return has_operator_not_equal    <T, U>::value; }
template<typename T, typename U> constexpr bool is_less_than_comparable()          noexcept { return has_operator_less         <T, U>::value; }
template<typename T, typename U> constexpr bool is_less_equal_than_comparable()    noexcept { return has_operator_less_equal   <T, U>::value; }
template<typename T, typename U> constexpr bool is_greater_than_comparable()       noexcept { return has_operator_greater      <T, U>::value; }
template<typename T, typename U> constexpr bool is_greater_equal_than_comparable() noexcept { return has_operator_greater_equal<T, U>::value; }

/// @}
/// @}

} // namespace rel_ops
} // namespace vccc

#endif // VCCC_UTILITY_CXX20_REL_OPS_HPP_
