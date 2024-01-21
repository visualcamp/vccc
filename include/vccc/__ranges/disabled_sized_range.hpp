//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_DISABLED_SIZED_RANGE_HPP_
#define VCCC_RANGES_DISABLED_SIZED_RANGE_HPP_

#include <type_traits>

namespace vccc {
namespace ranges {

template<typename T>
struct disabled_sized_range : std::false_type {};

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_DISABLED_SIZED_RANGE_HPP_
