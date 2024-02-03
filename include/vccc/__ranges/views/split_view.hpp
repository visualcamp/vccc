//
// Created by yonggyulee on 2/3/24.
//

#ifndef VCCC_RANGES_VIEWS_SPLIT_VIEW_HPP
#define VCCC_RANGES_VIEWS_SPLIT_VIEW_HPP

#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/search.hpp"
#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__functional/equal_to.hpp"
#include "vccc/__iterator/indirectly_comparable.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/empty.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/non_propagating_cache.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/subrange.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/single.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

/// @brief `%split_view` takes a \ref ranges::view "view" and a delimiter, and splits the view into subranges on the
/// delimiter.
///
/// @sa [std::ranges::split_view](https://en.cppreference.com/w/cpp/ranges/split_view)
template<typename V, typename Pattern>
class split_view : public view_interface<split_view<V, Pattern>> {
  template<typename R, bool = forward_range<R>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R>
  struct check_range<R, true>
      : conjunction<
            constructible_from<V, views::all_t<R>>,
            constructible_from<Pattern, single_view<range_value_t<R>>>
        > {};

 public:
  static_assert(view<V>::value, "Constraints not satisfied");
  static_assert(view<Pattern>::value, "Constraints not satisfied");
  static_assert(indirectly_comparable<iterator_t<V>, iterator_t<Pattern>, equal_to>::value, "Constraints not satisfied");

  class sentinel;
  class iterator;
  friend class sentinel;
  friend class iterator;

  class iterator {
   public:
    friend class sentinel;

    using iterator_concept = forward_iterator_tag;
    using iterator_category = input_iterator_tag;
    using value_type = subrange<iterator_t<V>>;
    using difference_type = range_difference_t<V>;
#if __cplusplus < 202002L
    using pointer = void;
    using reference = void;
#endif

    iterator() = default;

    VCCC_ADDRESSOF_CONSTEXPR iterator(split_view& parent, iterator_t<V> current, subrange<iterator_t<V>> next)
        : parent_(vccc::addressof(parent)), cur_(std::move(current)), next_(std::move(next)), trailing_empty_(false) {}

    constexpr const iterator_t<V> base() const {
      return cur_;
    }

    constexpr value_type operator*() const {
      return {cur_, next_.begin()};
    }

    constexpr iterator& operator++() {
      const auto last = ranges::end(parent_->base_);
      cur_ = next_.begin();
      if (cur_ != last) {
        cur_ = next_.end();
        if (cur_ == last) {
          trailing_empty_ = true;
          next_ = {cur_, cur_};
        } else {
          next_ = parent_->find_next(cur_);
        }
      } else {
        trailing_empty_ = false;
      }

      return *this;
    }

    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return x.cur_ == y.cur_ and x.trailing_empty_ == y.trailing_empty_;
    }

    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

   private:
    split_view* parent_ = nullptr;
    iterator_t<V> cur_{};
    subrange<iterator_t<V>> next_{};
    bool trailing_empty_ = false;
  };

  class sentinel {
   public:
    sentinel() = default;

    constexpr explicit sentinel(split_view& parent)
        : end_(ranges::end(parent.base_)) {}

    friend constexpr bool operator==(const iterator& x, const sentinel& y) {
      using namespace vccc::rel_ops;
      return x.cur_ == y.end_ && !x.trailing_empty_;
    }

    friend constexpr bool operator!=(const iterator& x, const sentinel& y) {
      return !(x == y);
    }

    friend constexpr bool operator==(const sentinel& y, const iterator& x) {
      return x == y;
    }

    friend constexpr bool operator!=(const sentinel& y, const iterator& x) {
      return !(x == y);
    }

   private:
    sentinel_t<V> end_ = sentinel_t<V>();
  };

  split_view() = default;

  constexpr explicit split_view(V base, Pattern pattern)
      : base_(std::move(base)), pattern_(std::move(pattern)) {}

  template<typename R, std::enable_if_t<check_range<R>::value, int> = 0>
  constexpr explicit split_view(R&& r, range_value_t<R> e)
      : base_(views::all(std::forward<R>(r)))
      , pattern_(single_view<remove_cvref_t<decltype(std::move(e))>>{std::move(e)}) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  constexpr iterator begin() {
    auto first = ranges::begin(base_);
    if (!cached_begin_.has_value())
      cached_begin_.emplace(find_next(first));
    return {*this, first, *cached_begin_};
  }

  template<typename V2 = V, std::enable_if_t<common_range<V2>::value, int> = 0>
  constexpr iterator end() {
    return iterator{*this, ranges::end(base_), {}};
  }

  template<typename V2 = V, std::enable_if_t<common_range<V2>::value == false, int> = 0>
  constexpr sentinel end() {
    return sentinel{*this};
  }

  constexpr subrange<iterator_t<V>> find_next(iterator_t<V> it) {
    using I = iterator_t<V>;

    auto last = ranges::end(base_);
    auto b_e = ranges::search(subrange<I>(it, last), pattern_);
    auto b = b_e.begin();
    auto e = b_e.end();

    if (b != last && ranges::empty(pattern_)) {
      ++b;
      ++e;
    }

    return {std::move(b), std::move(e)};
  }

 private:
  V base_{};
  Pattern pattern_{};
  non_propagating_cache<subrange<iterator_t<V>>> cached_begin_{};
};

namespace detail {

template<typename R, typename POV, bool = input_range<R>::value>
struct same_with_range_value : std::false_type {};
template<typename R, typename POV>
struct same_with_range_value<R, POV, true> : same_as<range_value_t<R>, remove_cvref_t<POV>> {};

template<typename R, std::enable_if_t<input_range<R>::value, int> = 0>
constexpr auto make_split_view_impl(R&& r, range_value_t<R> patteern, std::true_type /* range_value_t */) {
  return split_view<views::all_t<R>, single_view<range_value_t<R>>>(std::forward<R>(r), std::move(patteern));
}

template<typename R, typename P>
constexpr auto make_split_view_impl(R&& r, P&& pattern, std::false_type /* range_value_t */) {
  return split_view<views::all_t<R>, views::all_t<P>>(std::forward<R>(r), std::forward<P>(pattern));
}

} // namespace detail

template<typename R, typename P>
constexpr auto make_split_view(R&& r, P&& pattern) {
  return detail::make_split_view_impl(
      std::forward<R>(r), std::forward<P>(pattern), detail::same_with_range_value<R, P>{});
}

#if __cplusplus >= 201703L

template<typename R, typename P>
split_view(R&&, P&&)
    -> split_view<
          views::all_t<R>,
          std::conditional_t<
              detail::same_with_range_value<R, P>::value,
              single_view<range_value_t<R>>,
              views::all_t<P>
          >
        >;

#endif

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_SPLIT_VIEW_HPP
