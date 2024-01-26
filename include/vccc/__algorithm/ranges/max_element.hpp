//
// Created by YongGyu Lee on 01/08/24.
//

#ifndef VCCC_ALGORITHM_RANGES_MAX_ELEMENT_HPP
#define VCCC_ALGORITHM_RANGES_MAX_ELEMENT_HPP

#include <functional>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct max_element_niebloid {
 private:
  template<typename R, typename Proj, typename Comp, bool = forward_range<R>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R, typename Proj, typename Comp>
  struct check_range<R, Proj, Comp, true>
      : indirect_strict_weak_order<Comp, projected<iterator_t<R>, Proj> > {};

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
    while (++first != last) {
      if (vccc::invoke(comp, vccc::invoke(proj, *it), vccc::invoke(proj, *first)))
        it = first;
    }
    return it;
  }

  template<typename R, typename Proj = identity, typename Comp = less, std::enable_if_t<
      check_range<R, Proj, Comp>::value, int> = 0>
  constexpr borrowed_iterator_t<R>
  operator()(R&& r, Comp comp = {}, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::ref(comp), std::ref(proj));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::max_element_niebloid max_element{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_MAX_ELEMENT_HPP
