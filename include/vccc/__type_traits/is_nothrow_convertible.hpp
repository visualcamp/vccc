//
// Created by yonggyulee on 2024/01/15.
//

#ifndef VCCC_TYPE_TRAITS_IS_NOTHROW_CONVERTIBLE_HPP
#define VCCC_TYPE_TRAITS_IS_NOTHROW_CONVERTIBLE_HPP

#include <type_traits>

#include "vccc/__type_traits/bool_constant.hpp"

namespace vccc {
namespace detail {

template<typename From, typename To, bool = std::is_convertible<From, To>::value /* true */>
struct is_nothrow_convertible_impl : std::true_type {};

template<typename From, typename To>
struct is_nothrow_convertible_impl<From, To, false> :
#if __cplusplus < 201703L
    std::true_type
#else
    bool_constant<noexcept( std::declval<void(&)(To) noexcept>()(std::declval<From>()) )>
#endif
    {};
} // namespace detail

//! @addtogroup type_traits
//! @{

template<typename From, typename To>
struct is_nothrow_convertible : detail::is_nothrow_convertible_impl<From, To> {};

/// @}

} // namespace vccc

#endif // VCCC_TYPE_TRAITS_IS_NOTHROW_CONVERTIBLE_HPP
