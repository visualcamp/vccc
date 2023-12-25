//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_RANGES_BORROWED_RANGE_HPP_
#define VCCC_RANGES_BORROWED_RANGE_HPP_

#include <type_traits>

namespace vccc {
namespace ranges {

template<typename T>
struct enable_borrowed_range : std::false_type {};

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_BORROWED_RANGE_HPP_
