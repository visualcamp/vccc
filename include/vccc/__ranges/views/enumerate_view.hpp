//
// Created by yonggyulee on 1/22/24.
//

#ifndef VCCC_RANGES_VIEWS_ENUMERATE_VIEW_HPP
#define VCCC_RANGES_VIEWS_ENUMERATE_VIEW_HPP

#include <tuple>
#include <utility>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/move_constructible.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/bidirectional_range.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/detail/simple_view.hpp"
#include "vccc/__ranges/distance.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/range_rvalue_reference_t.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/maybe_const.hpp"
#if __cplusplus >= 201703L
#include "vccc/__ranges/views/all.hpp"
#endif
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = input_range<R>::value /* false */>
struct range_with_movable_reference : std::false_type {};
template<typename R>
struct range_with_movable_reference<R, true>
    : conjunction<
          move_constructible<range_reference_t<R>>,
          move_constructible<range_rvalue_reference_t<R>>
      > {};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename V>
class enumerate_view : public view_interface<enumerate_view<V>> {
 public:
  static_assert(view<V>::value, "Constraints not satisfied");
  static_assert(detail::range_with_movable_reference<V>::value, "Constraints not satisfied");

  template<bool Const> class iterator;
  template<bool Const> class sentinel;

  template<bool Const>
  class iterator {
    friend class enumerate_view;
    template<bool b>
    friend class sentinel;

    using Base = detail::maybe_const<Const, V>;

   public:
    using iterator_category = input_iterator_tag;
    using iterator_concept =
        std::conditional_t<
            random_access_range<Base>::value, random_access_iterator_tag,
        std::conditional_t<
            bidirectional_range<Base>::value, bidirectional_iterator_tag,
        std::conditional_t<
            forward_range<Base>::value, forward_iterator_tag,
            input_iterator_tag
        >>>;
    using difference_type = range_difference_t<Base>;
    using value_type = std::tuple<difference_type, range_value_t<Base>>;
   private:
    using reference_type = std::tuple<difference_type, range_reference_t<Base>>;
   public:
    iterator() = default;

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<iterator_t<V>, iterator_t<Base>>
    >::value, int> = 0>
    constexpr iterator(iterator<AntiConst> i)
        : current_(std::move(i.current_)), pos_(i.pos_) {}

   private:
    constexpr explicit iterator(iterator_t<Base> current, difference_type pos)
        : current_(std::move(current)), pos_(pos) {}

   public:
    constexpr const iterator_t<Base>& base() const& noexcept {
      return current_;
    }
    constexpr iterator_t<Base> base() && {
      return std::move(current_);
    }

    constexpr difference_type index() const noexcept {
      return pos_;
    }

    constexpr auto operator*() const {
      return reference_type(pos_, *current_);
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    constexpr auto operator[](difference_type n) const {
      return reference_type(pos_ + n, current_[n]);
    }

    constexpr iterator& operator++() {
      ++current_;
      ++pos_;
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
      --pos_;
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
      pos_ += n;
      return *this;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    constexpr iterator& operator-=(difference_type n) {
      current_ -= n;
      pos_ -= n;
      return *this;
    }

    friend constexpr bool operator==(const iterator& x, const iterator& y) noexcept {
      return x.pos_ == y.pos_;
    }

    friend constexpr bool operator!=(const iterator& x, const iterator& y) noexcept {
      return !(x == y);
    }

    friend constexpr bool operator<(const iterator& x, const iterator& y) noexcept {
      return x.pos_ < y.pos_;
    }

    friend constexpr bool operator<=(const iterator& x, const iterator& y) noexcept {
      return !(y < x);
    }

    friend constexpr bool operator>(const iterator& x, const iterator& y) noexcept {
      return y < x;
    }

    friend constexpr bool operator>=(const iterator& x, const iterator& y) noexcept {
      return !(x < y);
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr iterator operator+(const iterator& i, difference_type n) {
      auto temp = i;
      temp += n;
      return temp;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr iterator operator+(difference_type n, const iterator& i) {
      return i + n;
    }

    template<typename B = Base, std::enable_if_t<random_access_range<B>::value, int> = 0>
    friend constexpr iterator operator-(const iterator& i, difference_type n) {
      auto temp = i;
      temp -= n;
      return temp;
    }

    friend constexpr iterator operator-(const iterator& i, const iterator& j) noexcept {
      return i.pos_ - j.pos_;
    }

    friend constexpr auto iter_move(const iterator& i)
        noexcept(
            noexcept(ranges::iter_move(i.current_)) &&
            std::is_nothrow_move_constructible<
                range_rvalue_reference_t<Base>>::value
        )
    {
      return std::tuple<difference_type, range_rvalue_reference_t<Base>>(i.pos_, ranges::iter_move(i.current_));
    }

   private:
    iterator_t<Base> current_;
    difference_type pos_;
  };

  template<bool Const>
  class sentinel {
    using Base = std::conditional_t<Const, const V, V>;\
    friend class enumerate_view;

   public:
    sentinel() = default;

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<sentinel_t<V>, sentinel_t<Base>>
    >::value, int> = 0>
    constexpr sentinel(sentinel<AntiConst> other)
        : end_(std::move(other.end_)) {}

   private:
    constexpr explicit sentinel(sentinel_t<Base> end)
        : end_(std::move(end)) {}

   public:
    constexpr sentinel_t<Base> base() const {
      return end_;
    }

    friend constexpr bool operator==(const iterator<Const>& x, const sentinel& y) {
      using namespace vccc::rel_ops;
      return x.current_ == y.end_;
    }

    friend constexpr bool operator!=(const iterator<Const>& x, const sentinel& y) {
      return !(x == y);
    }

    friend constexpr bool operator==(const sentinel& y, const iterator<Const>& x) {
      return x == y;
    }

    friend constexpr bool operator!=(const sentinel& y, const iterator<Const>& x) {
      return !(x == y);
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sized_sentinel_for<
            sentinel_t<Base>,
            iterator_t<detail::maybe_const<OtherConst, V>>
        >
    >::value, int> = 0>
    friend constexpr range_difference_t<detail::maybe_const<OtherConst, V>>
    operator-(const iterator<OtherConst>& x, const sentinel& y) {
      return x.base() - y.base();
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sized_sentinel_for<
            sentinel_t<Base>,
            iterator_t<detail::maybe_const<OtherConst, V>>
        >
    >::value, int> = 0>
    friend constexpr range_difference_t<detail::maybe_const<OtherConst, V>>
    operator-(const sentinel& y, const iterator<OtherConst>& x) {
      return y.base() - x.base();
    }


   private:
    Base end_;
  };

  enumerate_view() = default;

  constexpr explicit enumerate_view(V base)
      : base_(std::move(base)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  template<typename V2 = V, std::enable_if_t<detail::simple_view<V2>::valule == false, int> = 0>
  constexpr iterator<false> begin() {
    return iterator<false>(ranges::begin(base_), 0);
  }

  template<typename V2 = V, std::enable_if_t<
      detail::range_with_movable_reference<const V2>::value, int> = 0>
  constexpr iterator<true> begin() const {
    return iterator<true>(ranges::begin(base_), 0);
  }

  template<typename V2 = V, std::enable_if_t<detail::simple_view<V2>::value == false, int> = 0>
  constexpr auto end() {
    return end_nonconst(conjunction<common_range<V2>, sized_range<V2>>{});
  }

  template<typename V2 = V, std::enable_if_t<
      detail::range_with_movable_reference<const V2>::value, int> = 0>
  constexpr auto end() const {
    return end_const(conjunction<common_range<const V2>, sized_range<const V2>>{});
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
  template<typename V2 = V>
  constexpr iterator<false> end_nonconst(std::true_type) {
    return iterator<false>(ranges::end(base_), ranges::distance(base_));
  }
  template<typename V2 = V>
  constexpr sentinel<false> end_nonconst(std::false_type) {
    return sentinel<false>(ranges::end(base_));
  }

  template<typename V2 = V>
  constexpr iterator<true> end_const(std::true_type) const {
    return iterator<true>(ranges::end(base_), ranges::distance(base_));
  }
  template<typename V2 = V>
  constexpr sentinel<true> end_const(std::false_type) const {
    return sentinel<true>(ranges::end(base_));
  }

  V base_;
};

#if __cplusplus >= 201703L

template<typename R>
enumerate_view(R&&) -> enumerate_view<views::all_t<R>>;

#endif

template<typename View>
struct enable_borrowed_range<enumerate_view<View>> : enable_borrowed_range<View> {};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ENUMERATE_VIEW_HPP
