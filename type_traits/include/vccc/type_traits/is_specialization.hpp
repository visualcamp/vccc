# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_SPECIALIZATION_HPP
# define VCCC_TYPE_TRAITS_IS_SPECIALIZATION_HPP
#
# include <type_traits>

namespace vccc{

template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

template<template<typename...> class Ref, typename ...Args>
using is_specialization_t = typename is_specialization<Ref<Args...>, Ref>::type;

template<template<typename...> class Ref, typename ...Args>
constexpr auto is_specialization_v = is_specialization<Ref<Args...>, Ref>::value;

}

# endif //VCCC_TYPE_TRAITS_IS_SPECIALIZATION_HPP