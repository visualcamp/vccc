//
// Created by yonggyulee on 2023/12/07.
//

#ifndef VCCC_MEMORY_TO_ADDRESS_HPP_
#define VCCC_MEMORY_TO_ADDRESS_HPP_

#include <memory>
#include <type_traits>

#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct has_to_address : std::false_type {};

template<typename T>
struct has_to_address<T, void_t<decltype(std::pointer_traits<T>::to_address(std::declval<const T&>()))>> : std::true_type {};

template<typename T, typename = void>
struct has_arrow : std::false_type {};

template<typename T>
struct has_arrow<T, void_t<decltype(std::declval<const T&>().operator->())>> : std::true_type {};

} // namespace detail

template<class T>
constexpr std::enable_if_t<!std::is_function<T>::value, T*>
to_address(T* p) noexcept {
  // static_assert(!std::is_function<T>::value, "T must not be a pointer to function");
  return p;
}

template<class T, std::enable_if_t<detail::has_to_address<T>::value, int> = 0>
constexpr auto to_address(const T& p) noexcept {
  return std::pointer_traits<T>::to_address(p);
}

template<class T, std::enable_if_t<!detail::has_to_address<T>::value && detail::has_arrow<T>::value, int> = 0>
constexpr auto to_address(const T& p) noexcept {
  return to_address(p.operator->());
}

} // namespace vccc

#endif // VCCC_MEMORY_TO_ADDRESS_HPP_
