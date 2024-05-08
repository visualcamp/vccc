//
// Created by YongGyu Lee on 5/8/24.
//

#ifndef VCCC_ALGORITHM_RANGES_CLAMP_HPP_
#define VCCC_ALGORITHM_RANGES_CLAMP_HPP_

#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct clamp_niebloid {
  template<typename T, typename Proj = identity, typename Comp = ranges::less, std::enable_if_t<conjunction<
      projectable<const T*, Proj>,
      indirect_strict_weak_order<Comp, projected<const T*, Proj>>
      >::value, int> = 0>
  constexpr const T& operator()(const T& v, const T& lo, const T& hi, Comp comp = {}, Proj proj = {}) const {
    auto&& pv = vccc::invoke(proj, v);

    if (vccc::invoke(comp, std::forward<decltype(pv)>(pv), vccc::invoke(proj, lo)))
      return lo;

    if (vccc::invoke(comp, vccc::invoke(proj, hi), std::forward<decltype(pv)>(pv)))
      return hi;

    return v;
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

/**
@brief clamps a value between a pair of boundary values

If `v` compares less than `lo`, returns `lo`; otherwise if `hi` compares less than `v`, returns `hi`; otherwise returns `v`.

The behavior is undefined if `lo` is greater than `hi`.

<H1>Complexity</H1>
At most two comparisons and three applications of the projection.

@sa [std::ranges::clamp](https://en.cppreference.com/w/cpp/algorithm/ranges/clamp)
@sa [std::clamp](https://en.cppreference.com/w/cpp/algorithm/clamp)
@sa vccc::clamp
 */
VCCC_INLINE_OR_STATIC constexpr detail::clamp_niebloid clamp{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_CLAMP_HPP_
