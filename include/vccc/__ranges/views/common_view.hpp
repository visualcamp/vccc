//
// Created by yonggyulee on 2/3/24.
//

#ifndef VCCC_RANGES_VIEWS_COMMON_VIEW_HPP
#define VCCC_RANGES_VIEWS_COMMON_VIEW_HPP

#include <type_traits>
#include <utility>

#include "vccc/__concepts/copyable.hpp"
#include "vccc/__iterator/common_iterator.hpp"
#include "vccc/__ranges/borrowed_range.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace views {

template<typename V>
class common_view : public view_interface<common_view<V>> {
  public:
  static_assert(common_range<V>::value == false, "Constraints not satisfied");
  static_assert(copyable<iterator_t<V>>::value, "Constraints not satisfied");

  common_view() = default;

  constexpr explicit common_view(V r)
      : base_(std::move(r)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  template<typename V2 = V, std::enable_if_t<conjunction<
      random_access_range<V2>,
      sized_range<V2>
  >::value, int> = 0>
  constexpr auto begin() {
    return ranges::begin(base_);
  }

  // template<typename V2 = V, std::enable_if_t<conjunction<
  //     random_access_range<V2>,
  //     sized_range<V2>
  // >::value == false, int> = 0>
  // constexpr auto begin() {
  //   return common_iterator
  // }


  private:

  V base_{};
};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_COMMON_VIEW_HPP
