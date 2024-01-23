//
// Created by yonggyulee on 2023/12/30.
//

#ifndef VCCC_SPAN_DYNAMIC_EXTENT_HPP
#define VCCC_SPAN_DYNAMIC_EXTENT_HPP

#include <cstddef>
#include <limits>

#include "vccc/__core/inline_or_static.hpp"

namespace vccc {

VCCC_INLINE_OR_STATIC constexpr std::size_t dynamic_extent = (std::numeric_limits<std::size_t>::max)();

} // namespace vccc

#endif // VCCC_SPAN_DYNAMIC_EXTENT_HPP
