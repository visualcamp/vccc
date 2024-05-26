//
// Created by yonggyulee on 2023/12/07.
//

#ifndef VCCC_MEMORY_TO_ADDRESS_HPP_
#define VCCC_MEMORY_TO_ADDRESS_HPP_

#include <memory>
#include <type_traits>

#include "vccc/__memory/pointer_traits.hpp"
#include "vccc/__type_traits/has_operator_arrow.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct has_to_address : std::false_type {};

template<typename T>
struct has_to_address<T, void_t<decltype(pointer_traits<T>::to_address(std::declval<const T&>()))>> : std::true_type {};

template<typename T>
constexpr auto to_address_fancy(const T& p, std::true_type /* has_to_address */ ) noexcept {
  return vccc::pointer_traits<T>::to_address(p);
}
template<typename T>
constexpr auto to_address_fancy(const T& p, std::false_type /* has_to_address */ ) noexcept;

} // namespace detail

/// @addtogroup iterator
/// @{

template<class T>
constexpr T* to_address(T* p) noexcept {
  static_assert(!std::is_function<T>::value, "T must not be a pointer to function");
  return p;
}

template<class T>
constexpr auto to_address(const T& p) noexcept {
  return detail::to_address_fancy(p, detail::has_to_address<T>{});
}

/// @}

namespace detail {

template<typename T>
constexpr auto to_address_fancy(const T& p, std::false_type /* has_to_address */ ) noexcept {
  return vccc::to_address(p.operator->());
}

} // namespace detail

} // namespace vccc

#endif // VCCC_MEMORY_TO_ADDRESS_HPP_
