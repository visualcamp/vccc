//
// Created by YongGyu Lee on 3/14/24.
//

#ifndef VCCC_EXPECTED_UNEXPECT_HPP_
#define VCCC_EXPECTED_UNEXPECT_HPP_

#if __cplusplus >= 202302L

#include <expected>

#else

#include "vccc/__core/inline_or_static.hpp"

#endif

namespace vccc {

#if __cplusplus >= 202302L

using unexpect_t = std::unexpect_t;
inline constexpr unexpect_t unexpect{};

#else

struct unexpect_t {
  explicit unexpect_t() = default;
};

VCCC_INLINE_OR_STATIC constexpr unexpect_t unexpect{};

#endif

} // namespace vccc

#endif // VCCC_EXPECTED_UNEXPECT_HPP_
