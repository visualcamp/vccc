//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ALGORITHM_SWAP_RANGES_HPP
#define VCCC_ALGORITHM_SWAP_RANGES_HPP

namespace vccc {
namespace ranges {

// TODO: Resolve circular header inclusion
template<typename I1, typename S1, typename I2, typename S2>
constexpr void swap_ranges(I1 first, S1 last, I2 first2, S2 last2) {

}

template<typename R1, typename R2>
constexpr void swap_ranges(R1&& r1, R2&& r2) {

}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_SWAP_RANGES_HPP
