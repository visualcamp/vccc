//
// Created by YongGyu Lee on 12/1/23.
//

#ifndef VCCC_FUNCTIONAL_HASH_ARRAY_HPP_
#define VCCC_FUNCTIONAL_HASH_ARRAY_HPP_

#include <climits>
#include <cstddef>
#include <type_traits>

namespace vccc {

//! @addtogroup functional
//! @{

template<typename T, std::size_t Bytes = sizeof(T) * CHAR_BIT> struct basic_FNV_prime;
template<typename T, std::size_t Bytes = sizeof(T) * CHAR_BIT> struct basic_FNV_offset_basis;

template<typename T> struct basic_FNV_prime<T, 32> : std::integral_constant<T, 16777619U> {};
template<typename T> struct basic_FNV_offset_basis<T, 32> : std::integral_constant<T, 2166136261U> {};

template<typename T> struct basic_FNV_prime<T, 64> : std::integral_constant<T, 1099511628211U> {};
template<typename T> struct basic_FNV_offset_basis<T, 64> : std::integral_constant<T, 14695981039346656037U> {};

// template<typename T> struct basic_FNV_prime<T, 128> : std::integral_constant<T, 309485009821345068724781371ULL> {};
// template<typename T> struct basic_FNV_offset_basis<T, 128> : std::integral_constant<T, 144066263297769815596495629667062367629ULL> {};

// template<typename T> struct basic_FNV_prime<T, 256>
//     : std::integral_constant<T, 374144419156711147060143317175368453031918731002211ULL> {};
// template<typename T> struct basic_FNV_offset_basis<T, 256>
//     : std::integral_constant<T, 100029257958052580907070968620625704837092796014241193945225284501741471925557ULL> {};

using FNV_prime = basic_FNV_prime<std::size_t>;
using FNV_offset_basis = basic_FNV_offset_basis<std::size_t>;

template<typename T>
std::size_t FNV_1(std::size_t value, const T& byte) {
  static_assert(std::is_integral<T>::value, "T must integral type");
  value = value * FNV_prime::value;
  value = value ^ static_cast<std::size_t>(byte);
  return value;
}

inline std::size_t FNV_1(std::size_t value, const unsigned char* const bytes, std::size_t size) {
  for (std::size_t i = 0; i < size; ++i) {
    value = FNV_1(value, bytes[i]);
  }
  return value;
}

template<typename T>
std::size_t FNV_1a(std::size_t value, const T& byte) {
  static_assert(std::is_integral<T>::value, "T must integral type");
  value = value ^ static_cast<std::size_t>(byte);
  value = value * FNV_prime::value;
  return value;
}

inline std::size_t FNV_1a(std::size_t value, const unsigned char* const bytes, std::size_t size) {
  for (std::size_t i = 0; i < size; ++i) {
    value = FNV_1a(value, bytes[i]);
  }
  return value;
}

template<typename T>
std::size_t hash_array(const T* bytes, std::size_t size) {
  return FNV_1a(FNV_offset_basis::value, reinterpret_cast<const unsigned char* const>(bytes), sizeof(T) * size);
}

template<typename T, std::size_t N>
std::size_t hash_array(const T(&bytes)[N]) {
  return hash_array(bytes, N);
}

//! @}

} // namespace vccc

#endif // VCCC_FUNCTIONAL_HASH_ARRAY_HPP_
