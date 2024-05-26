//
// Created by yonggyulee on 2024/01/06.
//

#ifndef VCCC_RANGES_FROM_RANGE_HPP_
#define VCCC_RANGES_FROM_RANGE_HPP_

#if __cplusplus >= 202302L
#include <ranges>
#endif

#include "vccc/__core/inline_or_static.hpp"


namespace vccc {

/// @addtogroup ranges
/// @{

#if __cplusplus >= 202302L && defined(__cpp_lib_containers_ranges)
using from_range_t = std::from_range_t;
#else
struct from_range_t {
  explicit from_range_t() = default;
};
#endif

VCCC_INLINE_OR_STATIC constexpr from_range_t from_range{};

/// @}

} // namespace vccc

#endif // VCCC_RANGES_FROM_RANGE_HPP_
