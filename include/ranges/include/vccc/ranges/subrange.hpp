//
// Created by yonggyulee on 2024/01/04.
//

#ifndef VCCC_RANGES_SUBRANGE_HPP
#define VCCC_RANGES_SUBRANGE_HPP

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/core/nodiscard.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/copyable.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/iterator/advance.hpp"
#include "vccc/iterator/bidirectional_iterator.hpp"
#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/iterator/sized_sentinel_for.hpp"
#include "vccc/iterator/sentinel_for.hpp"
#include "vccc/ranges/borrowed_range.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/enable_borrowed_range.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/sentinel_t.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/ranges/sized_range.hpp"
#include "vccc/ranges/subrange_kind.hpp"
#include "vccc/ranges/view_interface.hpp"
#include "vccc/tuple/tuple_like.hpp"
#include "vccc/type_traits/bool_constant.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/is_integer_like.hpp"
#include "vccc/type_traits/negation.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/utility/cxx20_rel_ops.hpp"
#include "vccc/utility/in_place.hpp"

namespace vccc {
namespace ranges {

namespace detail {

template<typename From, typename To>
struct convertible_to_non_slicing
    : conjunction<
          convertible_to<From, To>,
          disjunction<
              disjunction<
                  negation< std::is_pointer<From> >,
                  negation< std::is_pointer<To>   >
              >,
              std::is_convertible<std::remove_pointer_t<From>(*)[], std::remove_pointer_t<To>(*)[]>
          >
      > {};

template<typename T, typename U>
struct different_from : negation<same_as<std::decay_t<T>, std::decay_t<U>>> {};

template<typename T, bool = is_integer_like<T>::value /* false */>
struct make_unsigned_like {};
template<typename T>
struct make_unsigned_like<T, true> {
  using type = std::make_unsigned_t<T>;
};
template<typename T>
using make_unsigned_like_t = typename make_unsigned_like<T>::type;

template<typename T, typename U, typename V, bool = is_pair_like<T>::value /* true */>
struct pair_like_convertible_from
    : conjunction<
          negation< ranges::range<T> >,
          negation< std::is_reference<T> >,
          constructible_from<T, U, V>,
          convertible_to_non_slicing<U, std::tuple_element_t<0, T>>,
          convertible_to<V, std::tuple_element_t<1, T>>
      > {};

template<typename T, typename U, typename V>
struct pair_like_convertible_from<T, U, V, false> : std::false_type {};

template<typename R, bool = ranges::borrowed_range<R>::value /* true */>
struct borrowed_range_difference {
  using type = ranges::range_difference_t<R>;
};
template<typename R>
struct borrowed_range_difference<R, false> {};
template<typename R>
using borrowed_range_difference_t = typename borrowed_range_difference<R>::type;

template<
    typename I,
    typename S = I,
    ranges::subrange_kind K = sized_sentinel_for<S, I>::value ? ranges::subrange_kind::sized : ranges::subrange_kind::unsized
>
struct is_subrange_constructible
    : conjunction<
          input_or_output_iterator<I>,
          sentinel_for<S, I>,
          disjunction<
            bool_constant<(K == ranges::subrange_kind::sized)>,
            negation< sized_sentinel_for<S, I> >
          >
      > {};

template<typename I, typename S, bool Store /* false */>
struct subrange_size {
  constexpr subrange_size() = default;
  template<typename U>
  constexpr subrange_size(in_place_t, U) noexcept {}
};

template<typename I, typename S>
struct subrange_size<I, S, true> {
  constexpr subrange_size() = default;
  template<typename U>
  constexpr subrange_size(in_place_t, U n) : size_(n) {}

  std::make_unsigned_t<iter_difference_t<I>> size_ = 0;
};

template<typename I, typename S, typename R, bool = ranges::borrowed_range<R>::value /* true */>
struct subrange_ctor_range
    : conjunction<
          convertible_to_non_slicing<ranges::iterator_t<R>, I>,
          convertible_to<ranges::sentinel_t<R>, S>
      > {};

template<typename I, typename S, typename R>
struct subrange_ctor_range<I, S, R, false> : std::false_type {};


}

template<
    typename I,
    typename S = I,
    ranges::subrange_kind K = sized_sentinel_for<S, I>::value ? ranges::subrange_kind::sized : ranges::subrange_kind::unsized
>
class subrange
    : public ranges::view_interface<subrange<I, S, K>>
    , detail::subrange_size<I, S, (K == ranges::subrange_kind::sized && sized_sentinel_for<S, I>::value == false)>
{
  using store_size = bool_constant<(K == ranges::subrange_kind::sized && sized_sentinel_for<S, I>::value == false)>;
  using size_base = detail::subrange_size<I, S, store_size::value>;
 public:
  static_assert(input_or_output_iterator<I>::value, "Constraints not satisfied");
  static_assert(sentinel_for<S, I>::value, "Constraints not satisfied");
  static_assert(K == ranges::subrange_kind::sized || !sized_sentinel_for<S, I>::value, "Constraints not satisfied");

  subrange() = default;

  template<typename I2, std::enable_if_t<conjunction<
      detail::convertible_to_non_slicing<I2, I>,
      negation<store_size>
  >::value, int> = 0>
  constexpr subrange(I2 i, S s)
      : iterator_(std::move(i)), sentinel_(std::move(s)) {}

  template<typename I2, std::enable_if_t<conjunction<
      detail::convertible_to_non_slicing<I2, I>,
      bool_constant< K == ranges::subrange_kind::sized >
  >::value, int> = 0>
  constexpr subrange(I2 i, S s, detail::make_unsigned_like_t<iter_difference_t<I>> n)
      : iterator_(std::move(i)), sentinel_(std::move(s)), size_base(in_place, n) {}

  template<typename R, std::enable_if_t<conjunction<
      detail::different_from<subrange, R>,
      detail::subrange_ctor_range<I, S, R>,
      disjunction<
          negation<store_size>,
          ranges::sized_range<R>
      >
  >::value, int> = 0>
  constexpr subrange(R&& r)
      : subrange(r, static_cast<detail::make_unsigned_like_t<iter_difference_t<I>>>(ranges::size(r))) {}

  template<typename R, std::enable_if_t<conjunction<
      detail::subrange_ctor_range<I, S, R>,
      bool_constant< K == ranges::subrange_kind::sized >
  >::value, int> = 0>
  constexpr subrange(R&& r, detail::make_unsigned_like_t<iter_difference_t<I>> n)
      : subrange(ranges::begin(r), ranges::end(r), n) {}


  template<typename PairLike, std::enable_if_t<conjunction<
      negation< same_as<remove_cvref_t<PairLike>, subrange> >,
      detail::pair_like_convertible_from<PairLike, const I&, const S&>
  >::value, int> = 0>
  constexpr operator PairLike() const {
    return PairLike(iterator_, sentinel_);
  }


  template<typename I2 = I, std::enable_if_t<copyable<I2>::value, int> = 0>
  constexpr I begin() const {
    return iterator_;
  }

  template<typename I2 = I, std::enable_if_t<copyable<I2>::value == false, int> = 0>
  VCCC_NODISCARD constexpr I begin() {
    return std::move(iterator_);
  }

  constexpr S end() const {
    return sentinel_;
  }

  constexpr bool empty() const {
    using namespace vccc::rel_ops;
    return iterator_ == sentinel_;
  }

  template<ranges::subrange_kind K2 = K, std::enable_if_t<conjunction<
      bool_constant<K2 == ranges::subrange_kind::sized>,
      sized_sentinel_for<S, I>
  >::value, int> = 0>
  constexpr detail::make_unsigned_like_t<iter_difference_t<I>> size() const {
    return static_cast<detail::make_unsigned_like_t<iter_difference_t<I>>>(sentinel_ - iterator_);
  }

  template<ranges::subrange_kind K2 = K, std::enable_if_t<conjunction<
      bool_constant<K2 == ranges::subrange_kind::sized>,
      negation< sized_sentinel_for<S, I> >
  >::value, int> = 0>
  constexpr detail::make_unsigned_like_t<iter_difference_t<I>> size() const {
    return size_base::size_;
  }

  constexpr subrange& advance(iter_difference_t<I> n) {
    ranges::advance(iterator_, n, sentinel_);
    return *this;
  }

  template<typename I2 = I, std::enable_if_t<bidirectional_iterator<I2>::value, int> = 0>
  VCCC_NODISCARD constexpr subrange_kind prev(iter_difference_t<I> n = 1) const {
    auto tmp = *this;
    tmp.advance(-n);
    return tmp;
  }

  template<typename I2 = I, std::enable_if_t<forward_iterator<I2>::value, int> = 0>
  VCCC_NODISCARD constexpr subrange_kind next(iter_difference_t<I> n = 1) const& {
    auto tmp = *this;
    tmp.advance(n);
    return tmp;
  }

  template<typename I2 = I, std::enable_if_t<forward_iterator<I2>::value, int> = 0>
  VCCC_NODISCARD constexpr subrange_kind next(iter_difference_t<I> n = 1) && {
    advance(n);
    return std::move(*this);
  }

 private:
  I iterator_;
  S sentinel_;
};

// make-function for C++14

template<typename I, typename S, std::enable_if_t<conjunction<
    input_or_output_iterator<I>,
    sentinel_for<S, I>
>::value, int> = 0>
constexpr subrange<I, S>
make_subrange(I i, S s) {
  return subrange<I, S>(std::move(i), std::move(s));
}

template<typename I, typename S, std::enable_if_t<conjunction<
    input_or_output_iterator<I>,
    sentinel_for<S, I>
>::value, int> = 0>
constexpr subrange<I, S, ranges::subrange_kind::sized>
make_subrange(I i, S s, detail::make_unsigned_like_t<iter_difference_t<I>> n) {
  return subrange<I, S, ranges::subrange_kind::sized>(std::move(i), std::move(s), n);
}

template<typename R, std::enable_if_t<ranges::borrowed_range<R>::value, int> = 0>
constexpr subrange<ranges::iterator_t<R>, ranges::sentinel_t<R>,
             (ranges::sized_range<R>::value ||
              sized_sentinel_for<ranges::sentinel_t<R>, ranges::iterator_t<R>>::value) ?
                ranges::subrange_kind::sized : ranges::subrange_kind::unsized>
make_subrange(R&& r) {
  return {std::forward<R>(r)};
}

template<typename R, std::enable_if_t<ranges::borrowed_range<R>::value, int> = 0>
constexpr subrange<ranges::iterator_t<R>, ranges::sentinel_t<R>, ranges::subrange_kind::sized>
make_subrange(R&& r, detail::make_unsigned_like_t<ranges::range_difference_t<R>> n) {
  return {std::forward<R>(r), n};
}


#if __cplusplus >= 201703L

// template<typename R>
// subrange(R&&, detail::make_unsigned_like_t<range_difference_t<R>>) ->
//     subrange<ranges::iterator_t<R>, ranges::sentinel_t<R>, ranges::subrange_kind::sized>;
//
// template<typename I, typename S> subrange(I, S) -> subrange<I, S>;
//
// ^^^^^^^^^^ These two are ambiguous without constraints


template<typename I, typename S>
subrange(I, S, detail::make_unsigned_like_t<iter_difference_t<I>>) ->
    subrange<I, S, ranges::subrange_kind::sized>;

template<typename R>
subrange(R&&) ->
    subrange<ranges::iterator_t<R>, ranges::sentinel_t<R>,
             (ranges::sized_range<R>::value ||
              sized_sentinel_for<ranges::sentinel_t<R>, ranges::iterator_t<R>>::value) ?
                ranges::subrange_kind::sized : ranges::subrange_kind::unsized>;

#endif

namespace detail {

template<std::size_t N>
struct get_subrange;

template<> struct get_subrange<0> {
  template<typename I, typename S, ranges::subrange_kind K>
  static constexpr auto get(const ranges::subrange<I, S, K>& r) {
    return r.begin();
  }
  template<typename I, typename S, ranges::subrange_kind K>
  static constexpr auto get(ranges::subrange<I, S, K>&& r) {
    return std::move(r.begin());
  }
};

template<> struct get_subrange<1> {
  template<typename I, typename S, ranges::subrange_kind K>
  auto get(const ranges::subrange<I, S, K>& r) {
    return r.end();
  }
  template<typename I, typename S, ranges::subrange_kind K>
  auto get(ranges::subrange<I, S, K>&& r) {
    return std::move(r.end());
  }
};

} // namespace detail

template<std::size_t N, typename I, typename S, ranges::subrange_kind K,
    std::enable_if_t<((N == 0 && copyable<I>::value) || N == 1), int> = 0>
constexpr auto get(const ranges::subrange<I, S, K>& r) {
  return detail::get_subrange<N>::get(r);
}

template<std::size_t N, typename I, typename S, ranges::subrange_kind K,
    std::enable_if_t<(N < 2), int> = 0>
constexpr auto get(ranges::subrange<I, S, K>&& r) {
  return detail::get_subrange<N>::get(std::move(r));
}

template<typename I, typename S, vccc::ranges::subrange_kind K>
struct enable_borrowed_range<ranges::subrange<I, S, K>> : std::true_type {};

} // namespace ranges
} // namespace vccc

template<typename I, typename S, vccc::ranges::subrange_kind K>
struct std::tuple_size<vccc::ranges::subrange<I, S, K>> : std::integral_constant<std::size_t, 2> {};

namespace std {

using ::vccc::ranges::get;

template<typename I, typename S, vccc::ranges::subrange_kind K>
struct tuple_element<0, vccc::ranges::subrange<I, S, K>> { using type = I; };
template<typename I, typename S, vccc::ranges::subrange_kind K>
struct tuple_element<0, const vccc::ranges::subrange<I, S, K>> { using type = I; };
template<typename I, typename S, vccc::ranges::subrange_kind K>
struct tuple_element<1, vccc::ranges::subrange<I, S, K>> { using type = S; };
template<typename I, typename S, vccc::ranges::subrange_kind K>
struct tuple_element<1, const vccc::ranges::subrange<I, S, K>> { using type = S; };

} // namespace std

#endif // VCCC_RANGES_SUBRANGE_HPP
