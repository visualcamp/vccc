//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ALGORITHM_RANGES_SORT_HPP_
#define VCCC_ALGORITHM_RANGES_SORT_HPP_

#include <algorithm>
#include <functional>
#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/make_heap.hpp"
#include "vccc/__algorithm/ranges/sort_heap.hpp"
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
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct sort_niebloid {
  template<typename I, typename S, typename Comp = ranges::less, typename Proj = identity, std::enable_if_t<conjunction<
      random_access_iterator<I>,
      sentinel_for<S, I>,
      sortable<I, Comp, Proj>
  >::value, int> = 0>
  constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const {
    if (first == last)
      return first;

    auto last_iter = ranges::next(first, last);
    ranges::make_heap(first, last_iter, std::ref(comp), std::ref(proj));
    ranges::sort_heap(first, last_iter, std::ref(comp), std::ref(proj));

    return last_iter;
  }

  template<typename R, typename Comp = ranges::less, typename Proj = identity, std::enable_if_t<conjunction<
      random_access_range<R>,
      sortable<iterator_t<R>, Comp, Proj>
  >::value, int> = 0>
  constexpr borrowed_iterator_t<R> operator()(R&& r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::move(comp), std::move(proj));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::sort_niebloid sort{};

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_SORT_HPP_
