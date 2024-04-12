//
// Created by YongGyu Lee on 4/11/24.
//

#ifndef VCCC_ALGORITHM_RANGES_FIND_LAST_HPP_
#define VCCC_ALGORITHM_RANGES_FIND_LAST_HPP_

#include <functional>
#include <type_traits>

#include "vccc/__algorithm/ranges/detail/check_input.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirect_binary_predicate.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_subrange_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/subrange.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct find_last_niebloid {
  template<typename I, typename S, typename T, typename Proj = identity, std::enable_if_t<conjunction<
      input_iterator<I>,
      sentinel_for<S, I>,
      projectable<I, Proj>,
      indirect_binary_predicate<equal_to, projected<I, Proj>, const T*>
  >::value, int> = 0>
  constexpr subrange<I> operator()(I first, S last, const T& value, Proj proj = {}) const {
    I found{};

    for (; first != last; ++first) {
      if (vccc::invoke(proj, *first) == value)
        found = first;
    }

    if (found == I {})
      return {first, first};

    return {found, ranges::next(found, last)};
  }

  template<typename R, typename T, typename Proj = identity, std::enable_if_t<conjunction<
      input_range<R>,
      projectable<iterator_t<R>, Proj>,
      indirect_binary_predicate<equal_to, projected<iterator_t<R>, Proj>, const T*>
  >::value, int> = 0>
  constexpr borrowed_subrange_t<R> operator()(R&& r, const T& value, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), value, std::ref(proj));
  }
};

struct find_last_if_niebloid {
  template<typename I, typename S, typename Proj = identity, typename Pred, std::enable_if_t<conjunction<
      forward_iterator<I>,
      sentinel_for<S, I>,
      projectable<I, Proj>,
      indirect_unary_predicate<Pred, projected<I, Proj>>
  >::value, int> = 0>
  constexpr subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) const {
    I found{};
    for (; first != last; ++first) {
      if (vccc::invoke(pred, vccc::invoke(proj, *first))) {
        found = first;
      }
    }

    if (found == I {})
      return {first, first};

    return {found, ranges::next(found, last)};
  }

  template<typename R, typename Proj = identity, typename Pred, std::enable_if_t<conjunction<
      forward_range<R>,
      projectable<iterator_t<R>, Proj>,
      indirect_unary_predicate<Pred, projected<iterator_t<R>, Proj>>
  >::value, int> = 0>
  constexpr borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::ref(pred), std::ref(proj));
  }
};

struct find_last_if_not_niebloid {
  template<typename I, typename S, typename Proj = identity, typename Pred, std::enable_if_t<conjunction<
      forward_iterator<I>,
      sentinel_for<S, I>,
      projectable<I, Proj>,
      indirect_unary_predicate<Pred, projected<I, Proj>>
  >::value, int> = 0>
  constexpr subrange<I> operator()(I first, S last, Pred pred, Proj proj = {}) const {
    I found{};
    for (; first != last; ++first) {
      if (!vccc::invoke(pred, vccc::invoke(proj, *first))) {
        found = first;
      }
    }

    if (found == I {})
      return {first, first};

    return {found, ranges::next(found, last)};
  }

  template<typename R, typename Proj = identity, typename Pred, std::enable_if_t<conjunction<
      forward_range<R>,
      projectable<iterator_t<R>, Proj>,
      indirect_unary_predicate<Pred, projected<iterator_t<R>, Proj>>
  >::value, int> = 0>
  constexpr borrowed_subrange_t<R> operator()(R&& r, Pred pred, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::ref(pred), std::ref(proj));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::find_last_niebloid find_last{};
VCCC_INLINE_OR_STATIC constexpr detail::find_last_if_niebloid find_last_if{};
VCCC_INLINE_OR_STATIC constexpr detail::find_last_if_not_niebloid find_last_if_not{};

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_FIND_LAST_HPP_
