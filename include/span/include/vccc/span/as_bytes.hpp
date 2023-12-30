//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_SPAN_AS_BYTES_HPP
#define VCCC_SPAN_AS_BYTES_HPP

#include <cstddef>

#include "vccc/core/byte.hpp"
#include "vccc/span/dynamic_extent.hpp"
#include "vccc/span/span.hpp"

namespace vccc {
namespace detail {

template<typename T, std::size_t N>
struct as_bytes_size : std::integral_constant<std::size_t, sizeof(T) * N> {};
template<typename T>
struct as_bytes_size<T, dynamic_extent> : std::integral_constant<std::size_t, dynamic_extent> {};

} // namespace detail

/// @addtogroup span
/// @{

/**
@brief converts a `%span` into a view of its underlying bytes

Obtains a view to the object representation of the elements of the span `s`.

If `N` is `%vccc:dynamic_extent`, the extent of the returned span `S` is also `%vccc::dynamic_extent`; otherwise it is
`sizeof(T) * N`.
 */
template<typename T, std::size_t N>
constexpr span<const byte, detail::as_bytes_size<T, N>::value>
as_bytes(span<T, N> s) noexcept {
  return span<const byte, detail::as_bytes_size<T, N>::value>(
      reinterpret_cast<const byte*>(s.data()),
      s.size_bytes()
  );
}

/**
@brief @copybrief as_bytes
 */
template<typename T, std::size_t N, std::enable_if_t<std::is_const<T>::value == false, int> = 0>
constexpr span<byte, detail::as_bytes_size<T, N>::value>
as_writable_bytes(span<T, N> s) noexcept {
  return span<byte, detail::as_bytes_size<T, N>::value>(
      reinterpret_cast<byte*>(s.data()),
      s.size_bytes()
  );
}

/// @}

} // namespace vccc

#endif // VCCC_SPAN_AS_BYTES_HPP
