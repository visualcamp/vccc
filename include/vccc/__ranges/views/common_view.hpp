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
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/negation.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

/**
@brief Adapts a given \ref ranges::view "view" with different types for iterator/sentinel pair into a view that is also
a common_range. A common_view always has the same iterator/sentinel type
 */
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

  constexpr auto begin() {
    return begin_impl<V>(base_, conjunction<random_access_range<V>, sized_range<V>>{});
  }

  template<typename V2 = V, std::enable_if_t<range<const V2>::value, int> = 0>
  constexpr auto begin() const {
    return begin_impl<const V2>(base_, conjunction<random_access_range<const V2>, sized_range<const V2>>{});
  }

  constexpr auto end() {
    return end_impl<V>(base_, conjunction<random_access_range<V>, sized_range<V>>{});
  }

  template<typename V2 = V, std::enable_if_t<range<const V2>::value, int> = 0>
  constexpr auto end() const {
    return end_impl<const V>(base_, conjunction<random_access_range<const V>, sized_range<const V>>{});
  }

  template<typename V2 = V, std::enable_if_t<sized_range<V2>::value, int> = 0>
  constexpr auto size() {
    return ranges::size(base_);
  }

  template<typename V2 = V, std::enable_if_t<sized_range<const V2>::value, int> = 0>
  constexpr auto size() const {
    return ranges::size(base_);
  }

 private:
  template<typename V2, typename Base>
  static constexpr auto begin_impl(Base&& base, std::true_type /* random_and_sized */) {
    return ranges::begin(std::forward<Base>(base));
  }
  template<typename V2, typename Base>
  static constexpr auto begin_impl(Base&& base, std::false_type /* random_and_sized */) {
    return common_iterator<iterator_t<V2>, sentinel_t<V2>>(ranges::begin(std::forward<Base>(base)));
  }

  template<typename V2, typename Base>
  static constexpr auto end_impl(Base&& base, std::true_type /* random_and_sized */) {
    return ranges::begin(std::forward<Base>(base)) + ranges::size(std::forward<Base>(base));
  }
  template<typename V2, typename Base>
  static constexpr auto end_impl(Base&& base, std::false_type /* random_and_sized */) {
    return common_iterator<iterator_t<V2>, sentinel_t<V2>>(ranges::end(std::forward<Base>(base)));
  }

  V base_{};
};

#if __cplusplus >= 201703L

template<typename R>
common_view(R&&) -> common_view<views::all_t<R>>;

#endif

template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
constexpr common_view<views::all_t<R>> make_common_view(R&& r) {
  return common_view<views::all_t<R>>(std::forward<R>(r));
}


template<typename V>
struct enable_borrowed_range<common_view<V>> : enable_borrowed_range<V> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_COMMON_VIEW_HPP
