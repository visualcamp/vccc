//
// Created by yonggyulee on 1/18/24.
//

#ifndef VCCC_ALGORITHM_RANGES_FIND_HPP
#define VCCC_ALGORITHM_RANGES_FIND_HPP

#include <functional>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirect_binary_predicate.hpp"
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

struct find_niebloid {
 private:
  template<typename R, typename T, typename Proj, bool = input_range<R>::value>
  struct check_range : std::false_type {};
  template<typename R, typename T, typename Proj>
  struct check_range<R, T, Proj, true>
      : indirect_binary_predicate<ranges::equal_to, projected<iterator_t<R>, Proj>, const T*> {};

 public:
  template<typename I, typename S, typename T, typename Proj = identity, std::enable_if_t<conjunction<
      input_iterator<I>,
      sentinel_for<S, I>,
      indirect_binary_predicate<ranges::equal_to, projected<I, Proj>, const T*>
  >::value, int> = 0>
  constexpr I operator()(I first, S last, const T& value, Proj proj = {}) const {
    using namespace vccc::rel_ops;
    for (; first != last; ++first) {
      if (vccc::invoke(proj, *first) == value) {
        return first;
      }
    }
    return first;
  }

  template<typename R, typename T, typename Proj = identity, std::enable_if_t<
      check_range<R, T, Proj>::value, int> = 0>
  constexpr borrowed_iterator_t<R>
  operator()(R&& r, const T& value, Proj proj = {}) const {
    return (*this)(ranges::begin(r), ranges::end(r), value, std::ref(proj));
  }
};

} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::find_niebloid find{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_FIND_HPP
