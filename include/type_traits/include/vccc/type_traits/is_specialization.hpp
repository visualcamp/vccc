# /*
#  * Created by YongGyu Lee on 2020/12/08.
#  */
#
# ifndef VCCC_TYPE_TRAITS_IS_SPECIALIZATION_HPP
# define VCCC_TYPE_TRAITS_IS_SPECIALIZATION_HPP
#
# include <type_traits>

namespace vccc {

/**
@addtogroup type_traits
@{
    @defgroup is_specialization is_specialization
    @brief check if a type is specialization of another type
@}

@addtogroup is_specialization
@{

@code{.cpp}
std::vector<int> v;
std::cout << std::boolalpha;
std::cout << vccc::is_specialization<decltype(v), std::vector>::value << std::endl;
@endcode

*/
template<typename Test, template<typename...> class Ref>
struct is_specialization : std::false_type {};

template<template<typename...> class Ref, typename... Args>
struct is_specialization<Ref<Args...>, Ref> : std::true_type {};

//! @} is_specialization

} // namespace vccc

# endif // VCCC_TYPE_TRAITS_IS_SPECIALIZATION_HPP
