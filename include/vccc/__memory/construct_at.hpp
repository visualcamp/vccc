//
// Created by YongGyu Lee on 3/20/24.
//

#ifndef VCCC_MEMORY_CONSTRUCT_AT_HPP_
#define VCCC_MEMORY_CONSTRUCT_AT_HPP_

#include <new>
#include <type_traits>

#include "vccc/__core/constexpr.hpp"
#include "vccc/__utility/type_sequence.hpp"

namespace vccc {
namespace detail {

template<typename T, typename ArgPack, typename = void>
struct in_place_constructible_impl : std::false_type {};

template<typename T, typename... Args>
struct in_place_constructible_impl<T, type_sequence<Args...>,
    void_t<decltype(::new(std::declval<void*>()) T(std::declval<Args>()...))>> : std::true_type {};

template<typename T, typename... Args>
struct in_place_constructible
    : in_place_constructible_impl<T, type_sequence<Args...>> {};

} // namespace detail

/// @addtogroup memory
/// @{

template<typename T, typename... Args, std::enable_if_t<detail::in_place_constructible<T, Args...>::value, int> = 0>
constexpr T* construct_at(T* p, Args&&... args) noexcept(std::is_nothrow_constructible<T, Args...>::value) {
  return ::new (static_cast<void*>(p)) T(std::forward<Args>(args)...);
}

/// @}

} // namespace vccc

#endif // VCCC_MEMORY_CONSTRUCT_AT_HPP_
