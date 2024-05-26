//
// Created by yonggyulee on 2024/01/28.
//

#ifndef VCCC_RANGES_VIEWS_DROP_VIEW_HPP
#define VCCC_RANGES_VIEWS_DROP_VIEW_HPP

#include <algorithm>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/optional.hpp"
#include "vccc/__ranges/simple_view.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename V, bool = conjunction<simple_view<V>, sized_range<V>, random_access_range<V> >::value /* true */>
class drop_view_cached_begin {
 public:
  // Random access range. No cache required
  constexpr auto begin(const V& base, range_difference_t<V> count) const {
    return ranges::next(ranges::begin(base), count, ranges::end(base));
  }
};

template<typename V>
class drop_view_cached_begin<V, false> {
 public:
  // Not a random access range. Cache required
  constexpr auto begin(const V& base, range_difference_t<V> count) {
    if (!begin_.has_value()) {
      begin_ = ranges::next(ranges::begin(base), count, ranges::end(base));
    }
    return begin_;
  }

 private:
  optional<iterator_t<V>> begin_;
};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename V>
class drop_view
    : public ranges::view_interface<drop_view<V>>
    , public detail::drop_view_cached_begin<V>
{
  using begin_base = detail::drop_view_cached_begin<V>;

 public:
  static_assert(view<V>::value, "Constraints not satisfied");

  drop_view() = default;

  constexpr explicit drop_view(V base, range_difference_t<V> count)
      : base_(std::move(base)), count_(count) {}


  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& noexcept(std::is_nothrow_copy_constructible<V>::value) {
    return base_;
  }

  constexpr V base() && noexcept(std::is_nothrow_move_constructible<V>::value) {
    return std::move(base_);
  }

  template<typename V2 = V, std::enable_if_t<conjunction<
      simple_view<V2>,
      sized_range<const V2>,
      random_access_range<const V2>
  >::value == false, int> = 0>
  constexpr auto begin() {
    return begin_base::begin(base_, count_);
  }

  template<typename V2 = V, std::enable_if_t<conjunction<
      sized_range<const V2>,
      random_access_range<const V2>
  >::value, int> = 0>
  constexpr auto begin() const {
    return begin_base::begin(base_, count_);
  }

  template<typename V2 = V, std::enable_if_t<simple_view<V2>::value == false, int> = 0>
  constexpr auto end() {
    return ranges::end(base_);
  }

  template<typename V2 = V, std::enable_if_t<sized_range<const V2>::value, int> = 0>
  constexpr auto end() const {
    return ranges::end(base_);
  }

  template<typename V2 = V, std::enable_if_t<sized_range<V2>::value, int> = 0>
  constexpr auto size() {
    const auto s = ranges::size(base_);
    const auto c = static_cast<decltype(s)>(count_);
    return s < c ? 0 : s - c;
  }

  template<typename V2 = V, std::enable_if_t<sized_range<const V2>::value, int> = 0>
  constexpr auto size() const {
    const auto s = ranges::size(base_);
    const auto c = static_cast<decltype(s)>(count_);
    return s < c ? 0 : s - c;
  }

 private:
  V base_;
  range_difference_t<V> count_ = 0;
};

template<typename R>
constexpr drop_view<views::all_t<R>>
make_drop_view(R&& r, range_difference_t<R> count) {
  return drop_view<views::all_t<R>>(std::forward<R>(r), count);
}

#if __cplusplus >= 201703L

template<typename R>
drop_view(R&&, range_difference_t<R>) -> drop_view<views::all_t<R>>;

#endif

template<typename T>
struct enable_borrowed_range<drop_view<T>> : enable_borrowed_range<T> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_DROP_VIEW_HPP
