//
// Created by YongGyu Lee on 12/28/23.
//

#ifndef VCCC_ALGORITHM_RANGES_MIN_HPP
#define VCCC_ALGORITHM_RANGES_MIN_HPP

#include <functional>
#include <initializer_list>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__algorithm/ranges/min_element.hpp"
#include "vccc/__concepts/copyable.hpp"
#include "vccc/__functional/identity.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__functional/less.hpp"
#include "vccc/__iterator/indirect_strict_weak_order.hpp"
#include "vccc/__iterator/indirectly_copyable_storable.hpp"
#include "vccc/__iterator/projected.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct min_niebloid {
 private:
  template<typename R, typename Proj, typename Comp, bool = ranges::input_range<R>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R, typename Proj, typename Comp>
  struct check_range<R, Proj, Comp, true>
      : conjunction<
            indirect_strict_weak_order<Comp, projected<ranges::iterator_t<R>, Proj>>,
            indirectly_copyable_storable<ranges::iterator_t<R>, ranges::range_value_t<R>*>
        > {};

  template<typename R, typename Proj, typename Comp>
  constexpr ranges::range_value_t<R>
  min_range(R&& r, Comp comp, Proj proj, std::true_type /* forward_range */) const {
    return static_cast<ranges::range_value_t<R>>(*ranges::min_element(r, std::ref(comp), std::ref(proj)));
  }

  template<typename R, typename Proj, typename Comp>
  constexpr ranges::range_value_t<R>
  min_range(R&& r, Comp comp, Proj proj, std::false_type /* forward_range */) const {
    auto i = ranges::begin(r);
    auto s = ranges::end(r);
    ranges::range_value_t<R> m(*i);
    while (++i != s) {
      if (!vccc::invoke(comp, vccc::invoke(proj, *i), vccc::invoke(proj, m)))
        m = *i;
    }
    return m;
  }

 public:
  template<typename T, typename Proj = identity, typename Comp = less,
      std::enable_if_t<indirect_strict_weak_order<Comp, projected<const T*, Proj>>::value, int> = 0>
  constexpr const T&
  operator()(const T& a, const T& b, Comp comp = {}, Proj proj = {}) const {
    return vccc::invoke(comp, vccc::invoke(proj, a), vccc::invoke(proj, b)) ? a : b;
  }

  template<typename T, typename Proj = identity, typename Comp = less,
      std::enable_if_t<conjunction<
          copyable<T>,
          indirect_strict_weak_order<Comp, projected<const T*, Proj>>
      >::value, int> = 0>
  constexpr T
  operator()(std::initializer_list<T> r, Comp comp = {}, Proj proj = {}) const {
    return *ranges::min_element(r, std::ref(comp), std::ref(proj));
  }

  template<typename R, typename Proj = identity, typename Comp = less,
      std::enable_if_t<check_range<R, Proj, Comp>::value, int> = 0>
  constexpr ranges::range_value_t<R>
  operator()(R&& r, Comp comp = {}, Proj proj = {}) const {
    return this->min_range(std::forward<R>(r), std::ref(comp), std::ref(proj), ranges::forward_range<R>{});
  }
};
} // namespace detail

/// @addtogroup algorithm
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::min_niebloid min{};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_ALGORITHM_RANGES_MIN_HPP
