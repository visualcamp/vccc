//
// Created by YongGyu Lee on 12/1/23.
//

#ifndef VCCC_FUNCTIONAL_HASH_ARRAY_HPP_
#define VCCC_FUNCTIONAL_HASH_ARRAY_HPP_

#include <climits>
#include <cstddef>
#include <type_traits>

namespace vccc {

template<std::size_t Bytes = sizeof(std::size_t) * CHAR_BIT> struct FNV_offset_basis;
template<std::size_t Bytes = sizeof(std::size_t) * CHAR_BIT> struct FNV_prime;

template<> struct FNV_prime<32> : std::integral_constant<std::size_t, 16777619U> {};
template<> struct FNV_offset_basis<32> : std::integral_constant<std::size_t, 2166136261U> {};

template<> struct FNV_prime<64> : std::integral_constant<std::size_t, 1099511628211U> {};
template<> struct FNV_offset_basis<64> : std::integral_constant<std::size_t, 14695981039346656037U> {};

//template<> struct FNV_prime<128> : integral_constant<std::size_t, 309485009821345068724781371U> {};
//template<> struct FNV_offset_basis<128> : integral_constant<std::size_t, 144066263297769815596495629667062367629U> {};

template<typename T>
std::size_t FNV_1a(std::size_t value, const T& byte) {
  static_assert(std::is_integral<T>::value, "T must integral type");
  value = value ^ static_cast<std::size_t>(byte);
  value = value * FNV_prime<>::value;
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
  return FNV_1a(FNV_offset_basis<>::value, reinterpret_cast<const unsigned char* const>(bytes), sizeof(T) * size);
}

template<typename T, std::size_t N>
std::size_t hash_array(const T(&bytes)[N]) {
  return hash_array(bytes, N);
}

} // namespace vccc

#endif // VCCC_FUNCTIONAL_HASH_ARRAY_HPP_
