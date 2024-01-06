//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_RANGES_VIEWS_TAKE_VIEW_HPP
#define VCCC_RANGES_VIEWS_TAKE_VIEW_HPP

#include <algorithm>
#include <type_traits>
#include <utility>

#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/iterator/counted_iterator.hpp"
#include "vccc/iterator/sentinel_for.hpp"
#include "vccc/ranges/detail/simple_view.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/enable_borrowed_range.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/random_access_range.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/range_difference_t.hpp"
#include "vccc/ranges/sentinel_t.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/ranges/sized_range.hpp"
#include "vccc/ranges/view_interface.hpp"
#include "vccc/ranges/view.hpp"
#include "vccc/ranges/views/all.hpp"
#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/negation.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<bool C, typename T>
using add_const_if = std::conditional_t<C, const T, T>;

} // namespace detail

template<typename V>
class take_view : public ranges::view_interface<take_view<V>> {
 public:
  static_assert(view<V>::value, "Constraints not satisfied");

  template<bool Const>
  class sentinel {
    using Base = std::conditional_t<Const, const V, V>;
   public:
    sentinel() = default;

    constexpr explicit sentinel(ranges::sentinel_t<Base> end)
        : end_(std::move(end)) {}

    template<bool NoConst, std::enable_if_t<conjunction<
        bool_constant<Const>,
        bool_constant<Const != NoConst>,
        convertible_to<ranges::sentinel_t<V>, ranges::sentinel_t<Base>>
    >::value, int> = 0>
    constexpr sentinel(sentinel<NoConst> s)
        : end_(std::move(s.end_)) {}

    constexpr ranges::sentinel_t<Base> base() const {
      return end_;
    }

    friend constexpr bool
    operator==(const counted_iterator<ranges::iterator_t<Base>>& y, const sentinel& x) {
      using namespace vccc::rel_ops;
      return y.count() == 0 || y.base() == x.end_;
    }

    friend constexpr bool
    operator!=(const counted_iterator<ranges::iterator_t<Base>>& y, const sentinel& x) {
      return !(y == x);
    }

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<Const != AntiConst>,
        sentinel_for< ranges::sentinel_t<Base>,
                      ranges::iterator_t<detail::add_const_if<AntiConst, V>> >
    >::value, int> = 0>
    friend constexpr bool
    operator==(const counted_iterator<ranges::iterator_t<detail::add_const_if<AntiConst, V>>>& y, const sentinel& x) {
      using namespace vccc::rel_ops;
      return y.count() == 0 || y.base() == x.end_;
    }

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<Const != AntiConst>,
        sentinel_for< ranges::sentinel_t<Base>,
                      ranges::iterator_t<detail::add_const_if<AntiConst, V>> >
    >::value, int> = 0>
    friend constexpr bool
    operator!=(const counted_iterator<ranges::iterator_t<detail::add_const_if<AntiConst, V>>>& y, const sentinel& x) {
      using namespace vccc::rel_ops;
      return !(y == x);
    }

   private:
    sentinel_t<Base> end_;
  };

  take_view() = default;

  constexpr explicit take_view(V base, range_difference_t<V> count)
      : base_(std::move(base)), count_(count) {}


  template<typename T = V, std::enable_if_t<copy_constructible<T>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }


  template<typename T = V, std::enable_if_t<conjunction<
      negation< detail::simple_view<T> >,
      sized_range<T>,
      random_access_range<T>
  >::value, int> = 0>
  constexpr auto begin() {
    return ranges::begin(base_);
  }

  template<typename T = V, std::enable_if_t<conjunction<
      negation< detail::simple_view<T> >,
      sized_range<T>,
      negation< random_access_range<T> >
  >::value, int> = 0>
  constexpr auto begin() {
    using I = remove_cvref_t<iterator_t<T>>;
    return counted_iterator<I>(ranges::begin(base_), ranges::range_difference_t<T>(this->size()));
  }

  template<typename T = V, std::enable_if_t<conjunction<
      negation< detail::simple_view<T> >,
      negation< sized_range<T> >
  >::value, int> = 0>
  constexpr auto begin() {
    using I = remove_cvref_t<iterator_t<T>>;
    return counted_iterator<I>(ranges::begin(base_), count_);
  }

  template<typename T = const V, std::enable_if_t<conjunction<
      ranges::range<T>,
      sized_range<T>,
      random_access_range<T>
  >::value, int> = 0>
  constexpr auto begin() const {
    return ranges::begin(base_);
  }

  template<typename T = const V, std::enable_if_t<conjunction<
      ranges::range<T>,
      sized_range<T>,
      negation< random_access_range<T> >
  >::value, int> = 0>
  constexpr auto begin() const {
    using I = remove_cvref_t<iterator_t<T>>;
    return counted_iterator<I>(ranges::begin(base_), ranges::range_difference_t<T>(this->size()));
  }

  template<typename T = const V, std::enable_if_t<conjunction<
      ranges::range<T>,
      negation< sized_range<T> >
  >::value, int> = 0>
  constexpr auto begin() const {
    using I = remove_cvref_t<iterator_t<T>>;
    return counted_iterator<I>(ranges::begin(base_), count_);
  }


  template<typename T = V, std::enable_if_t<conjunction<
      negation< detail::simple_view<T> >,
      sized_range<T>,
      random_access_range<T>
  >::value, int> = 0>
  constexpr auto end() {
    return ranges::begin(base_) + ranges::range_difference_t<T>(this->size());
  }

  template<typename T = V, std::enable_if_t<conjunction<
      negation< detail::simple_view<T> >,
      sized_range<T>,
      negation< random_access_range<T> >
  >::value, int> = 0>
  constexpr default_sentinel_t end() {
    return default_sentinel;
  }

  template<typename T = V, std::enable_if_t<conjunction<
      negation< detail::simple_view<T> >,
      negation< sized_range<T> >
  >::value, int> = 0>
  constexpr sentinel<false> end() {
    return sentinel<false>(ranges::end(base_));
  }

  template<typename T = const V, std::enable_if_t<conjunction<
      ranges::range<T>,
      sized_range<T>,
      random_access_range<T>
  >::value, int> = 0>
  constexpr auto end() const {
    return ranges::begin(base_) + ranges::range_difference_t<T>(this->size());
  }

  template<typename T = V, std::enable_if_t<conjunction<
      ranges::range<T>,
      sized_range<T>,
      negation< random_access_range<T> >
  >::value, int> = 0>
  constexpr default_sentinel_t end() const {
    return default_sentinel;
  }

  template<typename T = V, std::enable_if_t<conjunction<
      ranges::range<T>,
      negation< sized_range<T> >
  >::value, int> = 0>
  constexpr sentinel<true> end() const {
    return sentinel<true>(ranges::end(base_));
  }

  template<typename T = V, std::enable_if_t<ranges::sized_range<T>::value, int> = 0>
  constexpr auto size() {
    auto n = ranges::size(base_);
    return (std::min)(n, static_cast<decltype(n)>(count_));
  }

  template<typename T = const V, std::enable_if_t<ranges::sized_range<T>::value, int> = 0>
  constexpr auto size() const {
    auto n = ranges::size(base_);
    return (std::min)(n, static_cast<decltype(n)>(count_));
  }

 private:
  V base_;
  range_difference_t<V> count_ = 0;
};

#if __cplusplus >= 201703L

template<typename R>
take_view(R&&, ranges::range_difference_t<R>) -> take_view<views::all_t<R>>;

#endif

template<typename T>
struct ranges::enable_borrowed_range<take_view<T>> : ranges::enable_borrowed_range<T> {};

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_TAKE_VIEW_HPP
