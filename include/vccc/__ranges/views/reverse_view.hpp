//
// Created by yonggyulee on 2/3/24.
//

#ifndef VCCC_RANGES_VIEWS_REVERSE_VIEW_HPP
#define VCCC_RANGES_VIEWS_REVERSE_VIEW_HPP

#include <iterator>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/bidirectional_range.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/non_propagating_cache.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/views/all.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename V, bool = common_range<V>::value /* true; no cache */>
struct reverse_view_cached_begin {
  constexpr std::reverse_iterator<iterator_t<V>> begin(V& base) {
    return std::make_reverse_iterator(ranges::end(base));
  }
};

template<typename V>
struct reverse_view_cached_begin<V, false> {
  constexpr std::reverse_iterator<iterator_t<V>> begin(V& base) {
    if (!cached_begin_.has_value()) {
      cached_begin_.emplace(std::make_reverse_iterator(ranges::next(ranges::begin(base), ranges::end(base))));
    }
    return cached_begin_.value();
  }

  non_propagating_cache<std::reverse_iterator<iterator_t<V>>> cached_begin_;
};

} // namespace detail

/// @addtogroup ranges
/// @{

/**
@brief A \ref ranges::range "range" adaptor that represents a view of underlying \ref ranges::view "view" with reversed
order.
 */
template<typename V>
class reverse_view
    : public view_interface<reverse_view<V>>
    , private detail::reverse_view_cached_begin<V>
{
  using cache_begin_base = detail::reverse_view_cached_begin<V>;
 public:
  static_assert(view<V>::value, "Constraints not satisfied");
  static_assert(bidirectional_range<V>::value, "Constraints not satisfied");

  reverse_view() = default;

  constexpr reverse_view(V r)
      : base_(std::move(r)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  constexpr std::reverse_iterator<iterator_t<V>> begin() {
    return cache_begin_base::begin(base_);
  }

  template<typename V2 = V, std::enable_if_t<common_range<const V2>::value, int> = 0>
  constexpr auto begin() const {
    return std::make_reverse_iterator(ranges::end(base_));
  }

  constexpr std::reverse_iterator<iterator_t<V>> end() {
    return std::make_reverse_iterator(ranges::begin(base_));
  }

  template<typename V2 = V, std::enable_if_t<common_range<const V2>::value, int> = 0>
  constexpr auto end() const {
    return std::make_reverse_iterator(ranges::begin(base_));
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
  V base_{};
};

#if __cplusplus >= 201703L

template<typename R>
reverse_view(R&&) -> reverse_view<views::all_t<R>>;

#endif

template<typename R>
constexpr reverse_view<views::all_t<R>> make_reverse_view(R&& r) {
  return reverse_view<views::all_t<R>>(std::forward<R>(r));
}

template<typename T>
struct enable_borrowed_range<reverse_view<T>> : enable_borrowed_range<T> {};


/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_REVERSE_VIEW_HPP
