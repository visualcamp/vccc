//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_TYPE_TRAITS_NUMERIC_HPP
#define VCPP_TYPE_TRAITS_NUMERIC_HPP

#include <type_traits>

namespace vc{


/** multiples_of */

template<typename T, T A, T B>
struct multiples_of : std::integral_constant<bool, A%B==0> {};

template<typename T, T A, T B>
using multiples_of_t = typename multiples_of<T, A, B>::type;

template<typename T, T A, T B>
constexpr bool multiples_of_v = multiples_of<T, A, B>::value;


/** is_odd */

template<typename T, T A>
struct is_odd : std::integral_constant<bool, A&1> {};

template<typename T, T A>
using is_odd_t = typename is_odd<T, A>::type;

template<typename T, T A>
constexpr bool is_odd_v = is_odd<T, A>::value;




}

#endif //VCPP_TYPE_TRAITS_NUMERIC_HPP
