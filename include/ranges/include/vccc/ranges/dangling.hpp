//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_RANGES_DANGLING_HPP
#define VCCC_RANGES_DANGLING_HPP

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{


/**
@brief a placeholder type indicating that an iterator or a subrange should not be returned since it would be dangling

@sa [std::ranges::dangling](https://en.cppreference.com/w/cpp/ranges/dangling)
*/

struct dangling {
  constexpr dangling() noexcept = default;

  template<typename... Args>
  constexpr dangling(Args&&...) noexcept {}
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_DANGLING_HPP
