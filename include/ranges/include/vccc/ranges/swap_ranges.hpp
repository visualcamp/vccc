//
// Created by yonggyulee on 2023/12/23.
//

#ifndef VCCC_RANGES_SWAP_RANGES_HPP_
#define VCCC_RANGES_SWAP_RANGES_HPP_

namespace vccc {
namespace ranges {

// TODO: Implement


template<typename I1, typename S1, typename I2, typename S2>
constexpr void
swap_ranges(I1 first1, S2 last1, I2 first2, S2 last2) noexcept {

}

template<typename R1, typename R2>
constexpr void
swap_ranges(R1&& r1, R2&& r2) noexcept {

}

// template< class I1, class I2 >
// using swap_ranges_result = ranges::in_in_result<I1, I2>;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SWAP_RANGES_HPP_
