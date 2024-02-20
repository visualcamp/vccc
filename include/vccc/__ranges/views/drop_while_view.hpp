//
// Created by YongGyu Lee on 2/13/24.
//

#ifndef VCCC_RANGES_VIEWS_DROP_WHILE_VIEW_HPP_
#define VCCC_RANGES_VIEWS_DROP_WHILE_VIEW_HPP_

#include <functional>
#include <type_traits>

#include "vccc/__algorithm/ranges/find_if_not.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/movable_box.hpp"
#include "vccc/__ranges/non_propagating_cache.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

template<typename V, typename Pred>
class drop_while_view : public view_interface<drop_while_view<V, Pred>> {
 public:
  static_assert(view<V>::value, "Constraints not satisfied");
  static_assert(input_range<V>::value, "Constraints not satisfied");
  static_assert(std::is_object<Pred>::value, "Constraints not satisfied");
  static_assert(indirect_unary_predicate<const Pred, iterator_t<V>>::value, "Constraints not satisfied");

  drop_while_view() = default;

  constexpr explicit drop_while_view(V base, Pred pred)
      : base_(std::move(base)), pred_(std::move(pred)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  constexpr const Pred& pred() const {
    return *pred_;
  }

  constexpr auto begin() {
    if (!cached_begin_) {
      cached_begin_.emplace(ranges::find_if_not(base_, std::cref(*pred_)));
    }
    return *cached_begin_;
  }

  constexpr auto end() {
    return ranges::end(base_);
  }

 private:
  V base_{};
  movable_box<Pred> pred_{};
  using begin_type = decltype(ranges::find_if_not(base_, std::cref(*pred_)));
  non_propagating_cache<begin_type> cached_begin_;
};

#if __cplusplus >= 201703L

template<typename R, typename Pred>
drop_while_view(R&&, Pred) -> drop_while_view<views::all_t<R>, Pred>;

#endif

template<typename R, typename Pred, std::enable_if_t<conjunction<
    has_typename_type<views::detail::all_t_impl<R>>,
    std::is_object<Pred>
>::value, int> = 0>
constexpr auto make_drop_while_view(R&& r, Pred&& pred) {
  return drop_while_view<views::all_t<R>, std::decay_t<Pred>>(std::forward<R>(r), std::forward<Pred>(pred));
}

template<typename T, typename Pred>
struct enable_borrowed_range<drop_while_view<T, Pred>> : enable_borrowed_range<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_DROP_WHILE_VIEW_HPP_
