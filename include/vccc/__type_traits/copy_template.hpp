//
// Created by YongGyu Lee on 5/16/24.
//

#ifndef VCCC_TYPE_TRAITS_COPY_TEMPLATE_HPP_
#define VCCC_TYPE_TRAITS_COPY_TEMPLATE_HPP_

#include "vccc/__type_traits/type_identity.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{

template<typename From, template<typename...> class To, template<typename, typename...> class Proj = type_identity_t>
struct copy_template;

template<template<typename...> class From, typename... T, template<typename...> class To, template<typename, typename...> class Proj>
struct copy_template<From<T...>, To, Proj> {
  using type = To<Proj<T>...>;
};

template<typename From, template<typename...> class To, template<typename, typename...> class Proj = type_identity_t>
using copy_template_t = typename copy_template<From, To, Proj>::type;

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_COPY_TEMPLATE_HPP_
