//
// Created by yonggyulee on 2023/10/27.
//

#ifndef VCCC_TYPE_TRAITS_COMMON_TYPE_HPP
#define VCCC_TYPE_TRAITS_COMMON_TYPE_HPP

#include <type_traits>

#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {

/**
 * @addtogroup type_traits
 * @{
 * @defgroup type_traits_common_type__class__Miscellaneous_transformations vccc::common_type
 * @brief determines the common type of a group of types
 *
 * @code{.cpp}
 * template< class... T >
 * struct common_type;
 * @endcode
 * Determines the common type among all types T..., that is the type all T...
 * can be implicitly converted to. If such a type exists (as determined
 * according to the rules below), the member type names that type. Otherwise,
 * there is no member type.
 *
 * @par Specializations
 * @parblock
 * Users may specialize common_type for types T1 and T2 if
 * at least one of T1 and T2 depends on a user-defined type, and
 * std::decay is an identity transformation for both T1 and T2.
 * If such a specialization has a member named type, it must be a public
 * and unambiguous member that names a cv-unqualified non-reference type
 * to which both T1 and T2 are explicitly convertible. Additionally,
 * vccc::common_type<T1, T2>::type and vccc::common_type<T2, T1>::type must
 * denote the same type.
 *
 * A program that adds common_type specializations in violation of these
 * rules has undefined behavior.
 *
 * The following specializations are already provided by the library:
 * - <strike>`vccc::common_type<std::pair>`</strike>
 * - @ref group_common_type_tuple_like "vccc::common_type<\a tuple-like> " @copybrief group_common_type_tuple_like
 *
 * @endparblock
 *
 * @par Notes
 * @parblock
 * For arithmetic types not subject to promotion, the common type may be
 * viewed as the type of the (possibly mixed-mode) arithmetic expression
 * such as `T0() + T1() + ... + Tn()`.
 * @endparblock
 *
 * @par Examples
 * @parblock
 * Demonstrates mixed-mode arithmetic on a user-defined class:
 * @code
#include <iostream>
#include <type_traits>

#include "vccc/type_traits"

template<class T>
struct Number { T n; };

template<class T, class U>
constexpr Number<vccc::common_type_t<T, U>>
    operator+(const Number<T>& lhs,
              const Number<U>& rhs)
{
    return {lhs.n + rhs.n};
}

void describe(const char* expr, const Number<int>& x)
{
    std::cout << expr << "  is  Number<int>{" << x.n << "}\n";
}

void describe(const char* expr, const Number<double>& x)
{
    std::cout << expr << "  is  Number<double>{" << x.n << "}\n";
}

int main()
{
    Number<int> i1 = {1}, i2 = {2};
    Number<double> d1 = {2.3}, d2 = {3.5};
    describe("i1 + i2", i1 + i2);
    describe("i1 + d2", i1 + d2);
    describe("d1 + i2", d1 + i2);
    describe("d1 + d2", d1 + d2);
}
 * @endcode
 * Output:
 * @code
i1 + i2  is  Number<int>{3}
i1 + d2  is  Number<double>{4.5}
d1 + i2  is  Number<double>{4.3}
d1 + d2  is  Number<double>{5.8}
 * @endcode
 * @endparblock
 *
 * @}
 */

/// @addtogroup type_traits_common_type__class__Miscellaneous_transformations
/// @{

template<typename ...T>
struct common_type;

namespace detail {

struct no_common_type {};

template<typename T, typename U>
using common_type_ternary = std::decay_t<decltype(false ? std::declval<T>() : std::declval<U>())>;

template<typename CR1, typename CR2>
struct common_type_test_3_1 {
  using type = common_type_ternary<CR1, CR2>;
};

// C++20 check
template<typename CR1, typename CR2, typename = void>
struct common_type_test_3 : no_common_type {};

template<typename CR1, typename CR2>
struct common_type_test_3<CR1, CR2, void_t<common_type_ternary<CR1, CR2>>>
    : common_type_test_3_1<CR1, CR2> {};

// Check if referencable
template<typename T1, typename T2,
    bool v1 = is_referencable<std::remove_reference_t<T1>>::value,
    bool v2 = is_referencable<std::remove_reference_t<T2>>::value>
struct common_type_test_2 : no_common_type {};

template<typename T1, typename T2>
struct common_type_test_2<T1, T2, true, true>
    : common_type_test_3<const std::remove_reference_t<T1>&, const std::remove_reference_t<T2>&> {};

template<typename T1, typename T2>
struct common_type_test_1_1 {
  using type = common_type_ternary<T1, T2>;
};

template<typename T1, typename T2, typename = void>
struct common_type_test_1 : common_type_test_2<T1, T2> {};

template<typename T1, typename T2>
struct common_type_test_1<T1, T2, void_t<common_type_ternary<T1, T2>>>
    : common_type_test_1_1<T1, T2> {};


template<bool HasType /* false */, typename CommonType, typename... Ts>
struct common_type_test_three_or_more : no_common_type {};

template<typename CommonType, typename... Ts>
struct common_type_test_three_or_more<true, CommonType, Ts...>
    : common_type<typename CommonType::type, Ts...> {};

} // namespace detail

template<>
struct common_type<> : detail::no_common_type {};

template<typename T>
struct common_type<T> : common_type<T, T> {};

template<typename T1, typename T2>
struct common_type<T1, T2> : detail::common_type_test_1<std::decay_t<T1>, std::decay_t<T2>> {};

template<typename T1, typename T2, typename ...Ts>
struct common_type<T1, T2, Ts...>
    : detail::common_type_test_three_or_more<
          has_typename_type<common_type<T1, T2>>::value,
          common_type<T1, T2>,
          Ts...> {};

template<typename ...T>
using common_type_t = typename common_type<T...>::type;

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_COMMON_TYPE_HPP
