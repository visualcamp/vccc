//
// Created by yonggyulee on 2023/12/21.
//

#ifndef VCCC_RANGES_SWAP_HPP_
#define VCCC_RANGES_SWAP_HPP_

#include "vccc/ranges/swap_ranges.hpp"

namespace vccc {
namespace ranges {
namespace detail_ranges_swap {

template<typename T>
void swap(T&, T&) = delete;

} // detail_ranges_swap


template<typename T, typename U>
constexpr void swap(T&& t, U&& u) {

}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SWAP_HPP_
