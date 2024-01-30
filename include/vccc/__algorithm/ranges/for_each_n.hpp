//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_ALGORITHM_RANGES_IN_FOR_EACH_N_HPP
#define VCCC_ALGORITHM_RANGES_IN_FOR_EACH_N_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__algorithm/ranges/in_fun_result.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirectly_unary_invocable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I, typename F>
using for_each_n_result = in_fun_result<I, F>;

namespace detail {

struct for_each_n_niebloid {
 private:
  template<typename I, typename Proj, typename Fun, bool = projectable<I, Proj>::value /* false */>
  struct check : std::false_type {};
  template<typename I, typename Proj, typename Fun>
  struct check<I, Proj, Fun, true> : indirectly_unary_invocable<Fun, projected<I, Proj>> {};

 public:
  template<typename I, typename Proj = identity, typename Fun, std::enable_if_t<conjunction<
      input_iterator<I>,
      check<I, Proj, Fun>
  >::value, int> = 0>
  constexpr for_each_n_result<I, Fun>
  operator()(I first, iter_difference_t<I> n, Fun f, Proj proj = {}) const {
    for (; n-- > 0; ++first) {
      vccc::invoke(f, vccc::invoke(proj, *first));
    }
    return {std::move(first), std::move(f)};
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::for_each_n_niebloid for_each_n{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_IN_FOR_EACH_N_HPP
