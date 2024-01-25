//
// Created by YongGyu Lee on 1/26/24.
//

#ifndef VCCC_ALGORITHM_CLAMP_HPP
#define VCCC_ALGORITHM_CLAMP_HPP

#include <functional>

namespace vccc {

/// @addtogroup algorithm
/// @{

template<typename T, typename Compare>
constexpr const T& clamp(const T& v, const T& lo, const T& hi, Compare comp) {
  return comp(v, lo) ? lo : comp(hi, v) ? hi : v;
}

template<typename T>
constexpr const T& clamp(const T& v, const T& lo, const T& hi) {
  return vccc::clamp(v, lo, hi, std::less<>{});
}

/// @}

} // namespace vccc

#endif // VCCC_ALGORITHM_CLAMP_HPP
