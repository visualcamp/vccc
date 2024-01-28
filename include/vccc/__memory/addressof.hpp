//
// Created by yonggyulee on 1/28/24.
//

#ifndef VCCC_MEMORY_ADDRESSOF_HPP_
#define VCCC_MEMORY_ADDRESSOF_HPP_

#include <memory>
#include <type_traits>

#include "vccc/__config.h"

# if __cplusplus < 201703L && !VCCC_CONSTEXPR_BUILTIN_ADDRESSOF_CXX14
#   define VCCC_ADDRESSOF_CONSTEXPR
# else
#   define VCCC_ADDRESSOF_CONSTEXPR constexpr
# endif

namespace vccc {

/// @addtogroup memory
/// @{

# if __cplusplus < 201703L
#  if VCCC_CONSTEXPR_BUILTIN_ADDRESSOF_CXX14
template<typename T>
constexpr T* addressof(T& t) noexcept {
  return __builtin_addressof(t);
}
#  else
template<typename T>
std::enable_if_t<std::is_object<T>::value, T*>
addressof(T& t) noexcept {
  return
    reinterpret_cast<T*>(
      &const_cast<char&>(
        reinterpret_cast<const volatile char&>(t)
      )
    );
}
template<typename T>
constexpr std::enable_if_t<std::is_object<T>::value == false, T*>
addressof(T& t) noexcept {
  return &t;
}
#  endif
# else
template<typename T>
constexpr T* addressof(T& t) noexcept {
  return std::addressof(t);
}
# endif

template<typename T>
constexpr const T* addressof(T&&) = delete;

/// @}

} // namespace vccc

#endif // VCCC_MEMORY_ADDRESSOF_HPP_
