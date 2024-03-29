//
// Created by yonggyulee on 1/24/24.
//

#ifndef VCCC_RANGES_VIEWS_FILTER_VIEW_HPP
#define VCCC_RANGES_VIEWS_FILTER_VIEW_HPP

#include <cassert>
#include <functional>
#include <memory>
#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/find_if.hpp"
#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__iterator/indirectly_swappable.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_swap.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__ranges/bidirectional_range.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/movable_box.hpp"
#include "vccc/__ranges/non_propagating_cache.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__ranges/range_rvalue_reference_t.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/has_operator_arrow.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename V, bool Cache = forward_range<V>::value>
class filter_view_cache;

template<typename V>
class filter_view_cache<V, true> {
 public:
  template<typename I, typename This, typename Base, typename Pred>
  constexpr I begin(This& thiz, Base& base, Pred& pred) {
    if (!begin_.has_value()) {
      begin_.emplace(ranges::find_if(base, std::ref(*pred)));
    }
    return I{thiz, begin_.value()};
  }

 private:
  non_propagating_cache<iterator_t<V>> begin_;
};

template<typename V>
class filter_view_cache<V, false> {
 public:
  template<typename I, typename This, typename Base, typename Pred>
  constexpr I begin(This& thiz, Base& base, Pred& pred) {
    return I{thiz, ranges::find_if(base, std::ref(*pred))};
  }
};

template<
    typename V,
    typename C = typename cxx20_iterator_traits<iterator_t<V>>::iterator_category,
    bool = forward_range<V>::value /* false */
>
struct filter_view_iterator_category {
#if __cplusplus < 202002L
  using iterator_category = iterator_ignore;
#endif
};

template<typename V, typename C>
struct filter_view_iterator_category<V, C, true> {
  using iterator_category =
     std::conditional_t<
         derived_from<C, bidirectional_iterator_tag>::value, bidirectional_iterator_tag,
     std::conditional_t<
         derived_from<C, forward_iterator_tag>::value, forward_iterator_tag,
         C
     >>;
};

template<typename I, bool = input_iterator<I>::value /* false */>
struct has_arrow : std::false_type {};
template<typename I>
struct has_arrow<I, true>
    : disjunction<
          std::is_pointer<I>,
          has_operator_arrow<I&>
      > {};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename V, typename Pred>
class filter_view : public view_interface<filter_view<V, Pred>>, detail::filter_view_cache<V> {
  V base_;
  movable_box<Pred> pred_;
  using cache_base = detail::filter_view_cache<V>;

 public:
  static_assert(input_range<V>::value, "Constraints not satisfied");
  static_assert(indirect_unary_predicate<Pred, iterator_t<V>>::value, "Constraints not satisfied");
  static_assert(view<V>::value, "Constraints not satisfied");
  static_assert(std::is_object<V>::value, "Constraints not satisfied");

  class iterator : public detail::filter_view_iterator_category<V> {
   public:
    using iterator_concept =
        std::conditional_t<
            bidirectional_range<V>::value, bidirectional_iterator_tag,
        std::conditional_t<
            forward_range<V>::value, forward_iterator_tag,
            input_iterator_tag
        >>;
    using value_type = range_value_t<V>;
    using difference_type = range_difference_t<V>;
#if __cplusplus < 202002L
    using pointer = void;
    using reference = range_reference_t<V>;
#endif

    iterator() = default;

    constexpr iterator(filter_view& parent, iterator_t<V> current)
        : current_(std::move(current)), parent_(vccc::addressof(parent)) {}

    constexpr const iterator_t<V>& base() const& noexcept {
      return current_;
    }

    constexpr iterator_t<V> base() && noexcept {
      return std::move(current_);
    }

    constexpr range_reference_t<V> operator*() const {
      return *current_;
    }

    template<typename V2 = V, std::enable_if_t<conjunction<
        detail::has_arrow<iterator_t<V2>>,
        copyable<iterator_t<V2>>
    >::value, int> = 0>
    constexpr iterator_t<V> operator->() const {
        return current_;
    }

    constexpr iterator& operator++() {
      current_ = ranges::find_if(
          std::move(++current_), ranges::end(parent_->base_), std::ref(*parent_->pred_));
      return *this;
    }

    template<typename V2 = V, std::enable_if_t<forward_range<V2>::value == false, int> = 0>
    constexpr void operator++(int) {
      return ++*this;
    }

    template<typename V2 = V, std::enable_if_t<forward_range<V2>::value, int> = 0>
    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    template<typename V2 = V, std::enable_if_t<bidirectional_range<V2>::value, int> = 0>
    constexpr iterator& operator--() {
      do {
        --current_;
      } while (!vccc::invoke(*parent_->pred_, *current_));
      return *this;
    }

    template<typename V2 = V, std::enable_if_t<bidirectional_range<V2>::value, int> = 0>
    constexpr iterator& operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    template<typename V2 = V, std::enable_if_t<equality_comparable<iterator_t<V2>>::value, int> = 0>
    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return x.current_ == y.current_;
    }

    template<typename V2 = V, std::enable_if_t<equality_comparable<iterator_t<V2>>::value, int> = 0>
    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

    // TODO: Solve "redefinition of 'iter_move' as different kind of symbol" in Android NDK 21.1.6352462
    // friend constexpr range_rvalue_reference_t<V>
    // iter_move(const iterator& i) noexcept(noexcept( ranges::iter_move(i.current_) )) {
    //   return ranges::iter_move(i.current_);
    // }

    // TODO: Solve "redefinition of 'iter_swap' as different kind of symbol"
    // TODO: Solve "const_cast from rvalue to reference type" in AppleClang 14.0.3.14030022
#if __cplusplus >= 202002L
    // friend constexpr void iter_swap(const iterator& x, const iterator& y)
    //     noexcept(noexcept(ranges::iter_swap(x.current_, y.current_)))
    //     requires(indirectly_swappable<iterator_t<V>>::value)
    // {
    //   ranges::iter_swap(x.current_, y.current_);
    // }
#endif

   private:
    iterator_t<V> current_;
    filter_view* parent_;
  };

  class sentinel {
    sentinel() = default;

    constexpr explicit sentinel(filter_view& parent)
        : end_(ranges::end(parent)) {}

    constexpr sentinel_t<V> base() const {
      return end_;
    }

    friend constexpr bool operator==(const iterator& x, const sentinel& y) {
      return x.base() == y.end_;
    }

    friend constexpr bool operator==(const sentinel& y, const iterator& x) {
      return x == y;
    }

    friend constexpr bool operator!=(const iterator& x, const sentinel& y) {
      return !(x == y);
    }

    friend constexpr bool operator!=(const sentinel& y, const iterator& x) {
      return !(x == y);
    }

   private:
    sentinel_t<V> end_;
  };

  friend class iterator;

  filter_view() = default;

  constexpr explicit filter_view(V base, Pred pred)
      : base_(std::move(base)), pred_(std::move(pred)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V>::value, int> = 0>
  constexpr V base() const & {
    return base_;
  }

  constexpr V base() const && {
    return std::move(base_);
  }

  constexpr const Pred& pred() const {
    assert(((void)"pred_ must contatin a value", pred_.has_value()));
    return *pred_;
  }

  constexpr iterator begin() {
    return cache_base::template begin<iterator>(*this, base_, pred_);
  }

  constexpr auto end() {
    return end(common_range<V>{});
  }

 private:
  constexpr iterator end(std::true_type /* common_range */) {
    return {*this, ranges::end(base_)};
  }
  constexpr sentinel end(std::false_type /* common_range */) {
    return {*this};
  }
};


#if __cplusplus >= 201703L

template<typename R, typename Pred>
filter_view(R&&, Pred) -> filter_view<views::all_t<R>, Pred>;

#endif

/// @}


} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_FILTER_VIEW_HPP
