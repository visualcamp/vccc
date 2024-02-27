//
// Created by YongGyu Lee on 2/24/24.
//

#ifndef VCCC_ALGORITHM_RANGES_LEXICOGRAPHICAL_COMPARE_HPP
#define VCCC_ALGORITHM_RANGES_LEXICOGRAPHICAL_COMPARE_HPP

#include <functional>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct lexicographical_compare_niebloid {
 private:
  template<typename I1, typename Proj1, typename I2, typename Proj2, typename Comp,
           bool = conjunction<projectable<I1, Proj1>, projectable<I2, Proj2>>::value /* false_type */>
  struct test_projectable_iterator : std::false_type {};
  template<typename I1, typename Proj1, typename I2, typename Proj2, typename Comp>
  struct test_projectable_iterator<I1, Proj1, I2, Proj2, Comp, true>
      : indirect_strict_weak_order<Comp, projected<I1, Proj1>, projected<I2, Proj2>> {};

  template<typename R1, typename Proj1, typename R2, typename Proj2, typename Comp,
           bool = conjunction<input_range<R1>, input_range<R2>>::value /* false_type */>
  struct test_projectable_range : std::false_type {};
  template<typename R1, typename Proj1, typename R2, typename Proj2, typename Comp>
  struct test_projectable_range<R1, Proj1, R2, Proj2, Comp, true>
      : test_projectable_iterator<iterator_t<R1>, Proj1, iterator_t<R2>, Proj2, Comp> {};

 public:
  template<
      typename I1, typename S1,
      typename I2, typename S2,
      typename Proj1 = identity, typename Proj2 = identity, typename Comp = ranges::less,
      std::enable_if_t<conjunction<
          input_iterator<I1>, sentinel_for<S1, I1>,
          input_iterator<I2>, sentinel_for<S2, I2>,
          test_projectable_iterator<I1, Proj1, I2, Proj2, Comp>
      >::value, int> = 0
  >
  constexpr bool operator()(I1 first1, S1 last1, I2 first2, S2 last2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    using namespace rel_ops;
    for (; (first1 != last1) && (first2 != last2); ++first1, (void) ++first2) {
      if (vccc::invoke(comp, vccc::invoke(proj1, *first1), vccc::invoke(proj2, *first2)))
        return true;
      if (vccc::invoke(comp, vccc::invoke(proj2, *first2), vccc::invoke(proj1, *first1)))
        return false;
    }
    return (first1 == last1) && (first2 != last2);
  }

  template<typename R1, typename R2, typename Proj1 = identity, typename Proj2 = identity, typename Comp = less, std::enable_if_t<
      test_projectable_range<R1, Proj1, R2, Proj2, Comp>
  ::value, int> = 0>
  constexpr bool operator()(R1&& r1, R2&& r2, Comp comp = {}, Proj1 proj1 = {}, Proj2 proj2 = {}) const {
    return (*this)(ranges::begin(r1), ranges::end(r1), ranges::begin(r2), ranges::end(r2), std::ref(comp), std::ref(proj1), std::ref(proj2));
  }

};

} // namespace detail

/// @addtogroup algorithm
/// @{

/// @brief returns `true` if one range is lexicographically less than another
VCCC_INLINE_OR_STATIC constexpr detail::lexicographical_compare_niebloid lexicographical_compare{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_LEXICOGRAPHICAL_COMPARE_HPP
