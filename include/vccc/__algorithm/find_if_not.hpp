//
// Created by yonggyulee on 1/18/24.
//

#ifndef VCCC_ALGORITHM_FIND_IF_NOT_HPP
#define VCCC_ALGORITHM_FIND_IF_NOT_HPP

#include <functional>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct find_if_not_niebloid {
 private:
  template<typename R, typename Proj, typename Pred, bool = input_range<R>::value>
  struct check_range : std::false_type {};
  template<typename R, typename Proj, typename Pred>
  struct check_range<R, Proj, Pred, true>
      : indirect_unary_predicate<Pred, projected<iterator_t<R>, Proj>> {};

 public:
  template<typename I, typename S, typename Proj = identity, typename Pred, std::enable_if_t<conjunction<
      input_iterator<I>,
      sentinel_for<S, I>,
      indirect_unary_predicate<Pred, projected<I, Proj>>
  >::value, int> = 0>
  constexpr I operator()(I first, S last, Pred pred, Proj proj = {}) const {
    using namespace vccc::rel_ops;
    for (; first != last; ++first) {
      if (!vccc::invoke(pred, vccc::invoke(proj, *first))) {
        return first;
      }
    }
    return first;
  }

  template<typename R, typename Proj = identity, typename Pred, std::enable_if_t<
      check_range<R, Proj, Pred>::value, int> = 0>
  constexpr borrowed_iterator_t<R>
  operator()(R&& r, Pred pred, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::ref(pred), std::ref(proj));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::find_if_not_niebloid find_if_not{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_FIND_IF_NOT_HPP
