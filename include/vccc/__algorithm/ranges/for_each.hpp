//
// Created by yonggyulee on 1/10/24.
//

#ifndef VCCC_ALGORITHM_RANGES_IN_FOR_EACH_HPP
#define VCCC_ALGORITHM_RANGES_IN_FOR_EACH_HPP

#include <functional>
#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/detail/check_input.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__algorithm/ranges/in_fun_result.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirectly_unary_invocable.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/end.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup algorithm
/// @{

template<typename I, typename F>
using for_each_result = in_fun_result<I, F>;

namespace detail {

struct for_each_niebloid {
  template<typename I, typename S, typename Proj = identity, typename Fun, std::enable_if_t<
      algo_check_unary_input_iterator<indirectly_unary_invocable, I, S, Proj, Fun>::value, int> = 0>
  constexpr for_each_result<I, Fun>
  operator()(I first, S last, Fun f, Proj proj = {}) const {
    for (; first != last; ++first) {
      vccc::invoke(f, vccc::invoke(proj, *first));
    }
    return {std::move(first), std::move(f)};
  }

  template<typename R, typename Proj = identity, typename Fun, std::enable_if_t<
      algo_check_unary_input_range<indirectly_unary_invocable, R, Proj, Fun>::value, int> = 0>
  constexpr for_each_result<borrowed_iterator_t<R>, Fun>
  operator()(R&& r, Fun f, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), std::move(f), std::ref(proj));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::for_each_niebloid for_each{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_IN_FOR_EACH_HPP
