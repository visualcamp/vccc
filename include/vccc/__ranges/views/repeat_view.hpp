//
// Created by yonggyulee on 2024/01/28.
//

#ifndef VCCC_RANGES_VIEWS_REPEAT_VIEW_HPP
#define VCCC_RANGES_VIEWS_REPEAT_VIEW_HPP

#include <cassert>
#include <cstdint>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/move_constructible.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__concepts/semiregular.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/unreachable_sentinel.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__ranges/movable_box.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/iota_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/is_integer_like.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

template<typename W, typename Bound = unreachable_sentinel_t>
class repeat_view : public view_interface<repeat_view<W, Bound>> {
 public:
  static_assert(move_constructible<W>::value, "Constraints not satisfied");
  static_assert(semiregular<Bound>::value, "Constraints not satisfied");
  static_assert(std::is_object<W>::value, "Constraints not satisfied");
  static_assert(same_as<W, std::remove_cv_t<W>>::value, "Constraints not satisfied");
  static_assert(disjunction<is_integer_like<Bound>,
    same_as<Bound, unreachable_sentinel_t>>::value, "Constraints not satisfied");

  class iterator {
    friend class repeat_view;
    using index_type = std::conditional_t<same_as<Bound, unreachable_sentinel_t>::value, std::ptrdiff_t, Bound>;

   public:
    using iterator_concept = random_access_iterator_tag;
    using iterator_category = random_access_iterator_tag;
    using value_type = W;
    using difference_type = std::conditional_t<
        is_signed_integer_like<index_type>::value, index_type, detail::iota_diff_t<index_type>>;
#if __cplusplus < 202002L
    using pointer = const W&;
    using reference =  const W&;
#endif

    constexpr iterator() : value_(nullptr), current_() {}

    constexpr const W& operator*() const noexcept {
      return *value_;
    }

    constexpr const W& operator[](difference_type n) const noexcept {
      return *(*this + n);
    }

    constexpr iterator& operator++() {
      ++current_;
      return *this;
    }

    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    constexpr iterator& operator--() {
      --current_;
      return *this;
    }

    constexpr iterator operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    constexpr iterator& operator+=(difference_type n) {
      current_ += n;
      return *this;
    }

    constexpr iterator& operator-=(difference_type n) {
      current_ -= n;
      return *this;
    }

    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return x.current_ == y.current_;
    }

    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

    friend constexpr bool operator<(const iterator& x, const iterator& y) {
      return x.current_ < y.current_;
    }

    friend constexpr bool operator<=(const iterator& x, const iterator& y) {
      return (x == y) || (x < y);
    }

    friend constexpr bool operator>(const iterator& x, const iterator& y) {
      return y < x;
    }

    friend constexpr bool operator>=(const iterator& x, const iterator& y) {
      return !(x < y);
    }

    friend constexpr iterator operator+(iterator i, difference_type n) {
      i += n;
      return i;
    }

    friend constexpr iterator operator+(difference_type n, iterator i) {
      i += n;
      return i;
    }

    friend constexpr iterator operator-(iterator i, difference_type n) {
      i -= n;
      return i;
    }

    friend constexpr difference_type operator-(const iterator& x, const iterator& y) {
      return static_cast<difference_type>(x.current_) - static_cast<difference_type>(y.current_);
    }

  private:
   constexpr explicit iterator(const W* value, index_type b = index_type())
       : value_(value), current_(b)
   {
     assert(((void)"B must be non negative", std::is_same<Bound, unreachable_sentinel_t>::value || (b >= 0)));
   }

    const W* value_;
    index_type current_;
  };

  repeat_view()
      : value_(W()), bound_(Bound()) {}

  constexpr explicit repeat_view(const W& value, Bound bound = Bound())
      : value_(value), bound_(bound){}

  constexpr explicit repeat_view(W&& value, Bound bound = Bound())
      : value_(std::move(value)), bound_(bound){}

  template<typename... WArgs, typename... BoundArgs, std::enable_if_t<conjunction<
      constructible_from<W, WArgs...>,
      constructible_from<Bound, BoundArgs...>
  >::value, int> = 0>
  constexpr repeat_view(std::piecewise_construct_t, std::tuple<WArgs...> value_args, std::tuple<BoundArgs...> bound_args = std::tuple<>())
      : repeat_view(std::piecewise_construct, std::move(value_args), std::move(bound_args),
                    std::index_sequence_for<WArgs...>{}, std::index_sequence_for<BoundArgs...>{}) {}

  VCCC_ADDRESSOF_CONSTEXPR iterator begin() const {
    return iterator(vccc::addressof(*value_));
  }

  template<typename B = Bound, std::enable_if_t<
      negation< same_as<B, unreachable_sentinel_t> >
  ::value, int> = 0>
  VCCC_ADDRESSOF_CONSTEXPR iterator end() const {
    return iterator(vccc::addressof(*value_), bound_);
  }

  template<typename B = Bound, std::enable_if_t<
      same_as<B, unreachable_sentinel_t>
  ::value, int> = 0>
  constexpr unreachable_sentinel_t end() const {
    return unreachable_sentinel;
  }

  template<typename B = Bound, std::enable_if_t<
      negation< same_as<B, unreachable_sentinel_t> >
  ::value, int> = 0>
  constexpr auto size() const {
    using R = std::make_unsigned_t<Bound>;
    return static_cast<R>(bound_);
  }

 private:
  template<typename WTuple, typename BoundTuple, std::size_t... I, std::size_t... J>
  constexpr repeat_view(
      std::piecewise_construct_t, WTuple&& value_args, BoundTuple&& bound_args,
      std::index_sequence<I...>, std::index_sequence<J...>)
      : value_(std::get<I>(std::forward<WTuple>(value_args))...), bound_(std::get<J>(std::forward<BoundTuple>(bound_args))...) {}

  movable_box<W> value_;
  Bound bound_;
};

#if __cplusplus >= 201703L

template<typename W, typename Bound>
repeat_view(W, Bound) -> repeat_view<W, Bound>;

#endif

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_REPEAT_VIEW_HPP
