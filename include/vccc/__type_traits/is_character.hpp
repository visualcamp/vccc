//
// Created by YongGyu Lee on 5/8/24.
//

#ifndef VCCC_TYPE_TRAITS_IS_CHARACTER_HPP_
#define VCCC_TYPE_TRAITS_IS_CHARACTER_HPP_

#include <type_traits>

#include "vccc/__config.h"

namespace vccc {

/// @addtogroup type_traits
/// @{

/**
 * @brief Check if type is character type
 * @tparam T
 */
template<typename T>
struct is_character : std::false_type {};

#if VCCC_HAS_TYPE_CHAR
template<> struct is_character<char> : std::true_type {};
template<> struct is_character<signed char> : std::true_type {};
template<> struct is_character<unsigned char> : std::true_type {};
#endif // VCCC_HAS_TYPE_CHAR

#if VCCC_HAS_TYPE_WCHAR_T
template<> struct is_character<wchar_t> : std::true_type {};
#endif // VCCC_HAS_TYPE_WCHAR_T

#if VCCC_HAS_TYPE_CHAR16_T
template<> struct is_character<char16_t> : std::true_type {};
#endif // VCCC_HAS_TYPE_CHAR16_T

#if VCCC_HAS_TYPE_CHAR32_T
template<> struct is_character<char32_t> : std::true_type {};
#endif // VCCC_HAS_TYPE_CHAR32_T

#if __cplusplus >= 202002L && VCCC_HAS_TYPE_CHAR8_T_CXX20
template<> struct is_character<char8_t> : std::true_type {};
#endif // __cplusplus >= 202002L && VCCC_HAS_TYPE_CHAR8_T_CXX20

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_CHARACTER_HPP_
