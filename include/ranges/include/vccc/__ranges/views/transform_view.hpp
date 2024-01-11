//
// Created by yonggyulee on 1/11/24.
//

#ifndef VCCC_RANGES_VIEWS_TRANSFORM_VIEW_HPP
#define VCCC_RANGES_VIEWS_TRANSFORM_VIEW_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__concepts/invocable.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/bidirectional_range.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename C>
struct transform_view_iterator_category_2 {
  using type = C;
};
template<>
struct transform_view_iterator_category_2<contiguous_iterator_tag> {
  using type = random_access_iterator_tag;
};

}

template<typename V, typename F>
class transform_view : public ranges::view_interface<transform_view<V, F>> {
 public:
  static_assert(ranges::input_range<V>::value, "Constraints not satisfied");
  static_assert(copy_constructible<F>::value, "Constraints not satisfied");
  static_assert(ranges::view<V>::value, "Constraints not satisfied");
  static_assert(std::is_object<F>::value, "Constraints not satisfied");
  static_assert(regular_invocable<F&, ranges::range_reference_t<V>>::value, "Constraints not satisfied");

 private:

  template<typename Base, bool v = forward_range<Base>::value /* false */>
  struct transform_view_iterator_category {};
  template<typename Base>
  struct transform_view_iterator_category<Base, true> {
    using iterator_category =
      std::conditional_t<
        std::is_reference<invoke_result_t<F&, ranges::range_reference_t<Base>>>::value,
          detail::transform_view_iterator_category_2<typename cxx20_iterator_traits<ranges::iterator_t<Base>>::iterator_category>,
          input_iterator_tag
      >;
  };

 public:
  template<bool Const> class iterator;
  template<bool Const> class sentinel;

  template<bool Const>
  class iterator : transform_view_iterator_category<std::conditional_t<Const, const V, V>> {
    using Parent = std::conditional_t<Const, const transform_view, transform_view>;
    using Base = std::conditional_t<Const, const V, V>;
   public:
    using iterator_concept =
      std::conditional_t<
        random_access_range<Base>::value, random_access_iterator_tag,
      std::conditional_t<
        bidirectional_range<Base>::value, bidirectional_iterator_tag,
      std::conditional_t<
        forward_range<Base>::value, forward_iterator_tag,
        input_iterator_tag
      >>>;
    using value_type = remove_cvref_t<invoke_result_t<F&, ranges::range_reference_t<Base>>>;
    using difference_type = ranges::range_difference_t<Base>;

    iterator() = default;

    constexpr iterator(Parent& parent, ranges::iterator_t<Base> current)
        : current_(std::move(current)), parent_(std::addressof(parent)) {}

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<ranges::iterator_t<V>, ranges::iterator_t<Base>>
    >::value, int> = 0>
    constexpr iterator(iterator<AntiConst> i)
        : current_(std::move(i.current_)), parent_(i.praent_) {}

    constexpr const ranges::iterator_t<Base>& base() const & noexcept { return current_; }
    constexpr ranges::iterator_t<Base> base() && { return std::move(current_); }

    constexpr decltype(auto) operator*() const {
      return vccc::invoke(*parent_->func_, *current_);
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    constexpr decltype(auto) operator[](difference_type n) const {
      return vccc::invoke(*parent_->func_, current_[n]);
    }

    constexpr iterator& operator++() {
      ++current_;
      return *this;
    }

    template<typename B = Base, std::enable_if_t<forward_range<B>::value == false, int> = 0>
    constexpr void operator++(int) {
      ++current_;
    }

    template<typename B = Base, std::enable_if_t<forward_range<B>::value, int> = 0>
    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    template<typename B = Base, std::enable_if_t<bidirectional_range<B>::value, int> = 0>
    constexpr iterator& operator--() {
      --current_;
      return *this;
    }

    template<typename B = Base, std::enable_if_t<bidirectional_range<B>::value, int> = 0>
    constexpr iterator operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    constexpr iterator& operator+=(difference_type n) {
      current_ += n;
      return *this;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    constexpr iterator& operator-=(difference_type n) {
      current_ -= n;
      return *this;
    }

    template<typename B = Base, std::enable_if_t<equality_comparable<iterator_t<B>>::value, int> = 0>
    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return x.current_ == y.current_;
    }

    template<typename B = Base, std::enable_if_t<equality_comparable<iterator_t<B>>::value, int> = 0>
    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator<(const iterator& x, const iterator& y) {
      return x.current_ < y.current_;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator>(const iterator& x, const iterator& y) {
      return y < x;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator<=(const iterator& x, const iterator& y) {
      return !(y < x);
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator>=(const iterator& x, const iterator& y) {
      return !(x < y);
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator+(iterator i, difference_type n) {
      return iterator{*i.parent_, i.current_ + n};
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator+(difference_type n, iterator i) {
      return iterator{*i.parent_, i.current_ + n};
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr bool operator-(iterator i, difference_type n) {
      return iterator{*i.parent_, i.current_ - n};
    }

    template<typename B = Base, std::enable_if_t<sized_sentinel_for<iterator_t<B>, iterator_t<B>>::value, int> = 0>
    friend constexpr difference_type operator-(const iterator& x, const iterator& y) {
      return x.current_ - y.current_;
    }

    friend constexpr decltype(auto) iter_move(const iterator& i)
        noexcept(noexcept(*i)) {
      return std::is_lvalue_reference<decltype(*i)>::value ? std::move(*i) : *i;
    }

   private:
    template<bool B>
    friend class sentinel;
    friend class transform_view;

    ranges::iterator_t<Base> current_;
    Parent* parent_ = nullptr;
  };

  template<bool Const>
  class sentinel {
    using Parent = std::conditional_t<Const, const transform_view, transform_view>;
    using Base = std::conditional_t<Const, const V, V>;

   public:
    sentinel() = default;

    constexpr explicit sentinel(sentinel_t<Base> end)
        : end_(std::move(end)) {}

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<sentinel_t<V>, sentinel_t<Base>>
    >::value, int> = 0>
    constexpr sentinel(sentinel<AntiConst> i)
        : end_(std::move(i.end_)) {}

    constexpr sentinel_t<Base> base() const {
      return end_;
    }

    friend constexpr bool operator==(const transform_view::iterator<Const>& x, const sentinel& y) {
      using namespace vccc::rel_ops;
      return x.current_ == y.end_;
    }

    friend constexpr bool operator!=(const transform_view::iterator<Const>& x, const sentinel& y) {
      return !(x == y);
    }

    template<typename B = Base, std::enable_if_t<sized_sentinel_for<sentinel_t<B>, iterator_t<B>>::value, int> = 0>
    friend constexpr range_difference_t<B>
    operator-(const iterator<Const>& x, const sentinel& y) {
      return x.current_ - y.end_;
    }

    template<typename B = Base, std::enable_if_t<sized_sentinel_for<sentinel_t<B>, iterator_t<B>>::value, int> = 0>
    friend constexpr range_difference_t<B>
    operator-(const sentinel& y, const iterator<Const>& x) {
      return y.end_ - x.current_;
    }

   private:
    sentinel_t<Base> end_;
  };

  transform_view() = default;

  constexpr explicit transform_view(V base, F func) : base_(std::move(base)), func_(func) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
   return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  constexpr iterator<false> begin() {
    return iterator<false>{*this, ranges::begin(base_)};
  }

  template<typename V2 = const V, typename F2 = const F, std::enable_if_t<conjunction<
      range<V2>,
      regular_invocable<F2&, ranges::range_reference_t<V2>>
  >::value, int> = 0>
  constexpr iterator<true> begin() const {
    return iterator<true>{*this, ranges::begin(base_)};
  }

  template<typename V2 = V, std::enable_if_t<common_range<V2>::value == false, int> = 0>
  constexpr sentinel<false> end() {
    return sentinel<false>{ranges::end(base_)};
  }

  template<typename V2 = V, std::enable_if_t<common_range<V2>::value, int> = 0>
  constexpr iterator<false> end() {
    return iterator<false>{*this, ranges::end(base_)};
  }

  template<typename V2 = const V, typename F2 = const F, std::enable_if_t<conjunction<
      range<V2>,
      negation<common_range<V2>>,
      regular_invocable<F2&, range_reference_t<V2>>
  >::value, int> = 0>
  constexpr sentinel<true> end() const {
    return sentinel<true>{ranges::end(base_)};
  }

  template<typename V2 = const V, typename F2 = const F, std::enable_if_t<conjunction<
      common_range<V2>,
      regular_invocable<F2&, range_reference_t<V2>>
  >::value, int> = 0>
  constexpr iterator<true> end() const {
    return iterator<true>{*this, ranges::end(base_)};
  }

  template<typename V2 = V, std::enable_if_t<sized_range<V2>::value, int> = 0>
  constexpr auto size() {
    return ranges::size(base_);
  }

  template<typename V2 = const V, std::enable_if_t<sized_range<V2>::value, int> = 0>
  constexpr auto size() const {
    return ranges::size(base_);
  }

 private:
  V base_;
  F func_;
};

template<typename R, typename F, std::enable_if_t<conjunction<
    input_range<views::all_t<R>>,
    copy_constructible<F>,
    std::is_object<F>,
    regular_invocable<F&, range_reference_t<views::all_t<R>>>
>::value, int> = 0>
constexpr transform_view<views::all_t<R>, F>
make_transform_view(R&& r, F f) {
  return transform_view<views::all_t<R>, F>(std::forward<R>(r), std::move(f));
}

#if __cplusplus >= 201703L

template<typename R, typename F>
transform_view(R&&, F) -> transform_view<views::all_t<R>, F>;

#endif

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_TRANSFORM_VIEW_HPP
