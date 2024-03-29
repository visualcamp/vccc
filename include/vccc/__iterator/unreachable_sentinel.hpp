//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_ITERATOR_UNREACHABLE_SENTINEL_HPP_
#define VCCC_ITERATOR_UNREACHABLE_SENTINEL_HPP_

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"

namespace vccc {

/// @addtogroup iterator
/// @{

/// @brief sentinel that always compares unequal to any \ref weakly_incrementable type
struct unreachable_sentinel_t {
  template<typename I>
  friend constexpr std::enable_if_t<weakly_incrementable<I>::value, bool>
  operator==(unreachable_sentinel_t, const I&) noexcept {
    return false;
  }
};

VCCC_INLINE_OR_STATIC constexpr unreachable_sentinel_t unreachable_sentinel{};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_UNREACHABLE_SENTINEL_HPP_
