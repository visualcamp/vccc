//
// Created by YongGyu Lee on 01/08/24.
//

#ifndef VCCC_ALGORITHM_MIN_ELEMENT_HPP
#define VCCC_ALGORITHM_MIN_ELEMENT_HPP

#include "vccc/core/inline_or_static.hpp"
#include "vccc/functional/identity.hpp"
#include "vccc/functional/invoke.hpp"
#include "vccc/functional/less.hpp"
#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/iterator/indirect_strict_weak_order.hpp"
#include "vccc/iterator/projected.hpp"
#include "vccc/iterator/sentinel_for.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/borrowed_iterator_t.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/forward_range.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct min_element_niebloid {
 private:
  template<typename R, typename Proj, typename Comp, bool = ranges::forward_range<R>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R, typename Proj, typename Comp>
  struct check_range<R, Proj, Comp, true>
      : indirect_strict_weak_order<Comp, projected<ranges::iterator_t<R>, Proj> > {};

 public:
  template<
    typename I,
    typename S,
    typename Proj = identity,
    typename Comp = ranges::less,
    std::enable_if_t<conjunction<
        forward_iterator<I>,
        sentinel_for<S, I>,
        indirect_strict_weak_order<Comp, projected<I, Proj>>
    >::value, int> = 0
  >
  constexpr I operator()(I first, S last, Comp comp = {}, Proj proj = {}) const {
    if (first == last)
      return last;

    auto it = first;
    for (++first; first != last; ++first) {
      if (!vccc::invoke(comp, vccc::invoke(proj, *it), vccc::invoke(proj, *first)))
        it = first;
    }
    return it;
  }

  template<typename R, typename Proj = identity, typename Comp = less, std::enable_if_t<
      check_range<R, Proj, Comp>::value, int> = 0>
  constexpr ranges::borrowed_iterator_t<R>
  operator()(R&& r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), comp, proj);
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::min_element_niebloid min_element{};

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_MIN_ELEMENT_HPP
