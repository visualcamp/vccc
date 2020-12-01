//
// Created by YongGyu Lee on 2020/12/01.
//

#ifndef VCPP_TYPE_TRAITS_DEFAULT_TYPE_TRAITS_HPP
#define VCPP_TYPE_TRAITS_DEFAULT_TYPE_TRAITS_HPP

#include "vcpp/type_traits/are.hpp"

namespace vc{


/** vtype_t */

template<typename T>
using vtype_t = typename T::value_type;


/** void_t */

#if __cplusplus <= 201402L
template<typename ...>
using void_t = void;
#else
template<typename ...Args> using void_t = ::std::void_t<Args...>;
#endif


/** is_iterator */

template<typename T, typename = void>
struct is_iterator : std::false_type {};

template<typename T>
struct is_iterator<T,
                    typename std::enable_if_t<
                      !std::is_same<
                        typename std::iterator_traits<T>::value_type,
                        void
                      >::value
                    >
                  > : std::true_type {
};

template<typename T>
using is_iterator_t = typename is_iterator<T>::type;

template<typename T>
constexpr auto is_iterator_v = is_iterator<T>::value;


/** iterable */

template<typename ...Ts>
constexpr auto iterable = are_v<is_iterator<Ts>...> || are_v<std::is_pointer<Ts>...>;


/** is_specialization */

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template<template<typename...> class Ref, typename ...Args>
using is_specialization_t = typename is_specialization<Ref<Args...>, Ref>::type;

template<template<typename...> class Ref, typename ...Args>
constexpr auto is_specialization_v = is_specialization<Ref<Args...>, Ref>::value;


/** is_container */

template<typename T, typename = void>
struct is_container : std::false_type {};

template<typename T>
struct is_container<T, void_t<decltype(std::declval<T>().begin()),
                              decltype(std::declval<T>().end()),
                              decltype(std::declval<T>().size())
>> : std::true_type {
};

template<typename T>
constexpr auto is_container_v = is_container<T>::value;


/** is_std_array */

template<typename ...>
struct is_std_array : std::false_type {};

template<typename T, std::size_t n>
struct is_std_array<std::array<T, n>> : std::true_type {};


/** decay_if_float_t */
// TODO: change double to biggest among input floating_point type
template<typename ...Types>
using decay_if_float_t = typename std::conditional_t<are_<std::is_same<Types, float>...>::value, float, double>;

}

#endif //VCPP_TYPE_TRAITS_DEFAULT_TYPE_TRAITS_HPP
