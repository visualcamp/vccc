//
// Created by YongGyu Lee on 02/01/24.
//

#ifndef VCCC_VARIANT_VARIANT_SIZE_HPP
#define VCCC_VARIANT_VARIANT_SIZE_HPP

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"

namespace vccc {

/// @addtogroup variant
/// @{

/// @cond ignored
template<typename... T>
class variant;
/// @endcond

template<typename T>
struct variant_size;

template<typename... Ts>
struct variant_size<variant<Ts...>> : std::integral_constant<std::size_t, sizeof...(Ts)> {};

template<typename T>
struct variant_size<const T> : std::integral_constant<std::size_t, variant_size<T>::value> {};

template<typename T>
VCCC_INLINE_OR_STATIC constexpr std::size_t variant_size_v = variant_size<T>::value;

/// @}

} // namespace vccc

#endif // VCCC_VARIANT_VARIANT_SIZE_HPP
