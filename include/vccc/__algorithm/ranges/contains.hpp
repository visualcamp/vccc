//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ALGORITHM_RANGES_CONTAINS_HPP_
#define VCCC_ALGORITHM_RANGES_CONTAINS_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/find.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/indirect_binary_predicate.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

namespace detail {

struct contains_niebloid {
  template<typename I, typename S, typename T, typename Proj = identity, std::enable_if_t<conjunction<
      input_iterator<I>,
      sentinel_for<S, I>,
      projectable<I, Proj>,
      indirect_binary_predicate<ranges::equal_to, projected<I, Proj>, const T*>
  >::value, int> = 0>
  constexpr bool operator()(I first, S last, const T& value, Proj proj = {}) const {
    return ranges::find(std::move(first), last, value, proj) != last;
  }

  template<typename R, typename T, typename Proj = identity, std::enable_if_t<conjunction<
      input_range<R>,
      projectable<iterator_t<R>, Proj>,
      indirect_binary_predicate<ranges::equal_to, projected<iterator_t<R>, Proj>, const T*>
  >::value, int> = 0>
  constexpr bool operator()(R&& r, const T& value, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::move(value), proj);
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::contains_niebloid contains{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_CONTAINS_HPP_
