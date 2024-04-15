//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ALGORITHM_RANGES_MAKE_HEAP_HPP_
#define VCCC_ALGORITHM_RANGES_MAKE_HEAP_HPP_

#include <algorithm>
#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/__iterator/random_access_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/sortable.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct make_heap_niebloid {
 public:
  template<typename I, typename S, typename Comp = ranges::less, typename Proj = identity, std::enable_if_t<conjunction<
      random_access_iterator<I>,
      sentinel_for<S, I>,
      sortable<I, Comp, Proj>
  >::value, int> = 0>
  constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const {
    auto last_iter = ranges::next(first, last);

    std::make_heap(std::move(first), last_iter, [&comp, &proj](auto&& lhs, auto&& rhs) {
      return vccc::invoke(
          comp,
          vccc::invoke(proj, std::forward<decltype(lhs)>(lhs)),
          vccc::invoke(proj, std::forward<decltype(rhs)>(rhs))
      );
    });
    return last_iter;
  }

  template<typename R, typename Comp = ranges::less, typename Proj = identity, std::enable_if_t<conjunction<
      random_access_range<R>,
      sortable<iterator_t<R>, Comp, Proj>
  >::value, int> = 0>
  constexpr iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), comp, proj);
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::make_heap_niebloid make_heap{};

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_MAKE_HEAP_HPP_
