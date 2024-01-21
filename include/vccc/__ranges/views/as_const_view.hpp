//
// Created by yonggyulee on 1/15/24.
//

#ifndef VCCC_RANGES_VIEWS_AS_CONST_VIEW_HPP
#define VCCC_RANGES_VIEWS_AS_CONST_VIEW_HPP

#include <utility>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__ranges/cbegin.hpp"
#include "vccc/__ranges/detail/simple_view.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/view.hpp"

namespace vccc {
namespace ranges {
namespace views {

template<typename V>
class as_const_view : public view_interface<as_const_view<V>> {
 public:
  static_assert(ranges::view<V>::value, "Constraints not satisfied");
  static_assert(ranges::input_range<V>::value, "Constraints not satisfied");

  as_const_view() = default;

  constexpr explicit as_const_view(V base)
      : base_(std::move(base)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  template<typename V2 = V, std::enable_if_t<ranges::detail::simple_view<V2>::value == false, int> = 0>
  constexpr auto begin() {
    return ranges::cbegin(base_);
  }

  template<typename V2 = const V, std::enable_if_t<ranges::range<V2>::value, int> = 0>
  constexpr auto begin() const {
    return ranges::cbegin(base_);
  }


 private:
  V base_;
};


} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_AS_CONST_VIEW_HPP
