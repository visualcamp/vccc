//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_AS_CONST_POINTER_HPP_
#define VCCC_RANGES_AS_CONST_POINTER_HPP_

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

template<typename T>
constexpr auto as_const_pointer(const T* p) noexcept {
  return p;
}

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_AS_CONST_POINTER_HPP_
