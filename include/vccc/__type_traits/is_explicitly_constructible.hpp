//
// Created by YongGyu Lee on 11/13/23.
//

#ifndef VCCC_TYPE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_HPP
#define VCCC_TYPE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_HPP

#include <type_traits>

#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/negation.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{

template<typename T, typename... Args>
struct is_explicitly_constructible :
    conjunction<
      std::is_constructible<T, Args...>,
      negation<std::is_convertible<Args..., T>>
    > {};

template<typename T, typename... Args>
using is_explicitly_constructible_t = typename is_explicitly_constructible<T, Args...>::type;

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_EXPLICITLY_CONSTRUCTIBLE_HPP
