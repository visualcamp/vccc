//
// Created by yonggyulee on 1/12/24.
//

#ifndef VCCC_ALGORITHM_RANGES_EQUAL_HPP
#define VCCC_ALGORITHM_RANGES_EQUAL_HPP

#include <functional>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__core/nodiscard.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/distance.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct equal_niebloid {
 private:
  template<typename R1, typename R2, typename Pred, typename Proj1, typename Proj2,
      bool = conjunction<input_range<R1>, input_range<R2>>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R1, typename R2, typename Pred, typename Proj1, typename Proj2>
  struct check_range<R1, R2, Pred, Proj1, Proj2, true>
      : indirectly_comparable<iterator_t<R1>, iterator_t<R2>, Pred, Proj1, Proj2> {};

  template<typename I1, typename S1, typename I2, typename S2>
  constexpr bool compare_size(I1 first1, S1 last1, I2 first2, S2 last2, std::true_type /* sized_sentinel_for */) const {
    return ranges::distance(first1, last1) == ranges::distance(first2, last2);
  }
  template<typename I1, typename S1, typename I2, typename S2>
  constexpr bool compare_size(I1, S1, I2, S2, std::false_type /* sized_sentinel_for */) const {
    return true;
  }

 public:
  template<
      typename I1, typename S1,
      typename I2, typename S2,
      typename Pred = ranges::equal_to,
      typename Proj1 = identity, typename Proj2 = identity,
      std::enable_if_t<conjunction<
          input_iterator<I1>, sentinel_for<S1, I1>,
          input_iterator<I2>, sentinel_for<S2, I2>,
          indirectly_comparable<I1, I2, Pred, Proj1, Proj2>
      >::value, int> = 0
  >
  VCCC_NODISCARD constexpr bool
  operator()(I1 first1, S1 last1, I2 first2, S2 last2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    if (!compare_size(first1, last1, first2, last2, conjunction<sized_sentinel_for<S1, I1>, sized_sentinel_for<S2, I2>>{}))
      return false;

    for (; first1 != last1; ++first1, (void)++first2) {
      if (!vccc::invoke(pred, vccc::invoke(proj1, *first1), vccc::invoke(proj2, *first2)))
        return false;
    }
    return true;
  }

  template<
      typename R1,
      typename R2,
      typename Pred = ranges::equal_to,
      typename Proj1 = vccc::identity, typename Proj2 = vccc::identity,
      std::enable_if_t<check_range<R1, R2, Pred, Proj1, Proj2>::value, int> = 0
  >
  VCCC_NODISCARD constexpr bool
  operator()(R1&& r1, R2&& r2, Pred pred = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2),
                   std::ref(pred), std::ref(proj1), std::ref(proj2));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::equal_niebloid equal{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_EQUAL_HPP
