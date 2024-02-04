//
// Created by YongGyu Lee on 11/3/23.
//

#ifndef VCCC_TYPE_TRAITS_IS_LIST_INITIALIZABLE_HPP
#define VCCC_TYPE_TRAITS_IS_LIST_INITIALIZABLE_HPP

#include <type_traits>

#include "vccc/__type_traits/void_t.hpp"

namespace vccc {

/// @addtogroup type_traits
/// @{

namespace detail {

template<typename To, typename From, typename = void>
struct is_list_initializable_impl : std::false_type {};

template<typename To, typename From>
struct is_list_initializable_impl<To, From, void_t<decltype(To{std::declval<From>()})>> : std::true_type {};

} // namespace detail

/// @defgroup type_traits_is_list_initializable__class__Type_relationships is_list_initializable
/// @brief checks if a type can be initialized from the other type using direct-list-initialization
/// @{
///
template<typename To, typename From>
struct is_list_initializable : detail::is_list_initializable_impl<To, From> {};

template<typename To, typename From>
using is_list_initializable_t = typename is_list_initializable<To, From>::type;

/// @}


/// @defgroup type_traits_is_copy_list_initializable__class__Type_relationships is_copy_list_initializable
/// @brief checks if a type can be initialized from the other type using copy-list-initialization
/// @{
///
template<typename To, typename From>
struct is_copy_list_initializable : std::conditional_t<is_list_initializable<To, From>::value, std::is_convertible<From, To>, std::false_type> {};

template<typename To, typename From>
using is_copy_list_initializable_t = typename is_copy_list_initializable<To, From>::type;

/// @}

/// @} type_traits

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_LIST_INITIALIZABLE_HPP
