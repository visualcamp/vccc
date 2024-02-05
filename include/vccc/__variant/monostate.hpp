//
// Created by YongGyu Lee on 11/3/23.
//

#ifndef VCCC_VARIANT_MONOSTATE_HPP
#define VCCC_VARIANT_MONOSTATE_HPP

#include <functional>

/// @addtogroup variant
/// @{

namespace vccc {


struct monostate {};

constexpr bool operator==(monostate, monostate) noexcept { return true; }
constexpr bool operator!=(monostate, monostate) noexcept { return false; }
constexpr bool operator< (monostate, monostate) noexcept { return false; }
constexpr bool operator> (monostate, monostate) noexcept { return false; }
constexpr bool operator<=(monostate, monostate) noexcept { return true; }
constexpr bool operator>=(monostate, monostate) noexcept { return true; }


} // namespace vccc

template<>
struct std::hash<::vccc::monostate> {
  std::size_t operator()(const ::vccc::monostate&) const noexcept {
    return 1998;
  }
};

/// @}

#endif // VCCC_VARIANT_MONOSTATE_HPP
