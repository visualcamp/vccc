//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_UTILITY_TO_UNDERLYING_HPP
#define VCCC_UTILITY_TO_UNDERLYING_HPP

#include <type_traits>

namespace vccc {

/// @addtogroup utility
/// @{


/**
@brief converts an enumeration to its underlying type

@sa [std::to_underlying](https://en.cppreference.com/w/cpp/utility/to_underlying)
 */

template<typename Enum>
constexpr std::underlying_type_t<Enum>& as_const(Enum e) noexcept {
  return static_cast<std::underlying_type_t<Enum>>(e);
}

/// @}

} // namespace vccc

#endif // VCCC_UTILITY_TO_UNDERLYING_HPP
