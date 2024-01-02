//
// Created by yonggyulee on 2024/01/03.
//

#ifndef VCCC_RANGES_DISTANCE_HPP
#define VCCC_RANGES_DISTANCE_HPP

#include <type_traits>

#include "vccc/core/inline_or_static.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/iterator/sentinel_for.hpp"
#include "vccc/iterator/sized_sentinel_for.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/range_difference_t.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/negation.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

using vccc::detail::return_category;

struct distance_niebloid {
  template<typename I, typename S, std::enable_if_t<conjunction<
      sentinel_for<S, I>,
      negation< sized_sentinel_for<S, I> >
  >::value, int> = 0>
  constexpr iter_difference_t<I> distance(I first, S last) const {
    using namespace vccc::rel_ops;
    iter_difference_t<I> result = 0;
    while (first != last) {
      ++first;
      ++result;
    }
    return result;
  }

  template<typename I, typename S, std::enable_if_t<sized_sentinel_for<S, std::decay_t<I>>::value, int> = 0>
  constexpr iter_difference_t<std::decay_t<I>> distance(I&& first, S last) const {
    return last - static_cast<const std::decay_t<I>&>(first);
  }

  template<typename R, std::enable_if_t<ranges::sized_range<remove_cvref_t<R>>::value, int> = 0>
  constexpr ranges::range_difference_t<R> distance(R&& r) const {
    return static_cast<ranges::range_difference_t<R>>(ranges::size(std::forward<R>(r)));
  }

  template<typename R, std::enable_if_t<ranges::sized_range<remove_cvref_t<R>>::value == false, int> = 0>
  constexpr ranges::range_difference_t<R> distance(R&& r) const {
    return (*this)(ranges::begin(r), ranges::end(r));
  }

};

} // namespace detail

inline namespace niebloid {

/// @addtogroup ranges
/// @{


VCCC_INLINE_OR_STATIC constexpr detail::distance_niebloid distance{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_DISTANCE_HPP
