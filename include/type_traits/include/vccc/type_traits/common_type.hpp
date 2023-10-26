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
 * @brief determines the common type of a group of types
 *
 * Determines the common type among all types T..., that is the type all T...
 * can be implicitly converted to. If such a type exists (as determined
 * according to the rules below), the member type names that type. Otherwise,
 * there is no member type.
 *
 *
 *
 * @tparam T
 */
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

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_COMMON_TYPE_HPP
