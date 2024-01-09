//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_TYPE_TRAITS_IS_INTEGER_LIKE_HPP_
#define VCCC_TYPE_TRAITS_IS_INTEGER_LIKE_HPP_

#include <limits>
#include <type_traits>

#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{


/**
 *  @brief Check if type models
 *  [integer-like](https://en.cppreference.com/w/cpp/iterator/weakly_incrementable#Integer-like_types)
 *  type (C++ 20 requirement)
 */
template<typename T>
struct is_integer_like
    : disjunction<
        std::is_integral<T>,
        conjunction<
          bool_constant<std::numeric_limits<T>::is_integer>,
          bool_constant<std::numeric_limits<T>::is_specialized>
        >
      > {};

template<typename T>
struct is_signed_integer_like
    : disjunction<
        conjunction<
          std::is_integral<T>,
          std::is_signed<T>
        >,
        conjunction<
          bool_constant<std::numeric_limits<T>::is_integer>,
          bool_constant<std::numeric_limits<T>::is_signed>,
          bool_constant<std::numeric_limits<T>::is_specialized>
        >
      > {};

template<typename T>
struct is_unsigned_integer_like
    : disjunction<
        conjunction<
          std::is_integral<T>,
          std::is_unsigned<T>
        >,
        conjunction<
          bool_constant<std::numeric_limits<T>::is_integer>,
          bool_constant<!std::numeric_limits<T>::is_signed>,
          bool_constant<std::numeric_limits<T>::is_specialized>
        >
      > {};

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_INTEGER_LIKE_HPP_
