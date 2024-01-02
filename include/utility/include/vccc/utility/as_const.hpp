//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_UTILITY_AS_CONST_HPP
#define VCCC_UTILITY_AS_CONST_HPP

#include <type_traits>

namespace vccc {

/// @addtogroup utility
/// @{


/**
@brief obtains a reference to `const` to its argument

@sa [std::as_const](https://en.cppreference.com/w/cpp/utility/as_const)
 */

template<typename T>
constexpr std::add_const_t<T>& as_const(T& t) noexcept {
  return t;
}

template<typename T>
void as_const(T&&) = delete;

/// @}

} // namespace vccc

#endif // VCCC_UTILITY_AS_CONST_HPP
