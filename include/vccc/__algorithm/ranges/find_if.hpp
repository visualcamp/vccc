//
// Created by yonggyulee on 1/18/24.
//

#ifndef VCCC_ALGORITHM_RANGES_FIND_IF_HPP
#define VCCC_ALGORITHM_RANGES_FIND_IF_HPP

#include <functional>
#include <type_traits>

#include "vccc/__algorithm/ranges/detail/check_input.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct find_if_niebloid {
  template<typename I, typename S, typename Proj = identity, typename Pred, std::enable_if_t<
      algo_check_unary_input_iterator<indirect_unary_predicate, I, S, Proj, Pred>
  ::value, int> = 0>
  constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const {
    using namespace vccc::rel_ops;
    for (; first != last; ++first) {
      if (vccc::invoke(pred, vccc::invoke(proj, *first))) {
        return first;
      }
    }
    return first;
  }

  template<typename R, typename Proj = identity, typename Pred, std::enable_if_t<
      algo_check_unary_input_range<indirect_unary_predicate, R, Proj, Pred>
  ::value, int> = 0>
  constexpr borrowed_iterator_t<R>
  operator()(R&& r, Pred pred, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::ref(pred), std::ref(proj));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::find_if_niebloid find_if{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_FIND_IF_HPP
