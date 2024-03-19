//
// Created by YongGyu Lee on 3/19/24.
//

#ifndef VCCC_ALGORITHM_RANGES_SET_INTERSECTION_HPP_
#define VCCC_ALGORITHM_RANGES_SET_INTERSECTION_HPP_

#include <utility>

#include "vccc/__algorithm/ranges/in_in_out_result.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/mergeable.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"
#include "vccc/__ranges/borrowed_iterator_t.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

template< class I1, class I2, class O >
using set_intersection_result = ranges::in_in_out_result<I1, I2, O>;

namespace detail {

struct set_intersection_niebloid {
 private:
  template<typename R1, typename R2, typename O, typename Comp, typename Proj1, typename Proj2,
           bool = conjunction<input_range<R1>, input_range<R2>>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R1, typename R2, typename O, typename Comp, typename Proj1, typename Proj2>
  struct check_range<R1, R2, O, Comp, Proj1, Proj2, true>
      : conjunction<
          weakly_incrementable<O>,
          mergeable<iterator_t<R1>, iterator_t<R2>, O, Comp, Proj1, Proj2>
      > {};

 public:
  template<
      typename I1, typename S1,
      typename I2, typename S2,
      typename O,
      typename Comp = ranges::less,
      typename Proj1 = identity,
      typename Proj2 = identity,
      std::enable_if_t<conjunction<
          input_iterator<I1>, sentinel_for<S1, I1>,
          input_iterator<I2>, sentinel_for<S2, I2>,
          weakly_incrementable<O>,
          mergeable<I1, I2, O, Comp, Proj1, Proj2>
      >::value, int> = 0>
  constexpr set_intersection_result<I1, I2, O>
  operator()(I1 first1, S1 last1, I2 first2, S2 last2, O result, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    while (first1 != last1 && first2 != last2) {
      if (vccc::invoke(comp, vccc::invoke(proj1, *first1), vccc::invoke(proj2, *first2))) {
        ++first1;
      } else if (vccc::invoke(comp, vccc::invoke(proj2, *first2), vccc::invoke(proj1, *first1))) {
        ++first2;
      } else {
        *result = *first1;
        ++first1;
        ++first2;
        ++result;
      }
    }
    return {
      ranges::next(std::move(first1), std::move(last1)),
      ranges::next(std::move(first2), std::move(last2)),
      std::move(result)
    };
  }

  template<
      typename R1, typename R2,
      typename O,
      typename Comp = ranges::less,
      typename Proj1 = identity,
      typename Proj2 = identity,
      std::enable_if_t<
          check_range<R1, R2, O, Comp, Proj1, Proj2>
      ::value, int> = 0>
  constexpr set_intersection_result<borrowed_iterator_t<R1>, borrowed_iterator_t<R2>, O>
  operator()(R1&& r1, R2&& r2, O result, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (*this)(ranges::begin(r1), ranges::end(r1),
                   ranges::begin(r2), ranges::end(r2),
                   std::move(result), std::move(comp),
                   std::move(proj1), std::move(proj2));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::set_intersection_niebloid set_intersection{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_SET_INTERSECTION_HPP_
