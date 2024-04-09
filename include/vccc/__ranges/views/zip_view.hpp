//
// Created by YongGyu Lee on 4/8/24.
//

#ifndef VCCC_RANGES_VIEWS_ZIP_VIEW_HPP_
#define VCCC_RANGES_VIEWS_ZIP_VIEW_HPP_

#include <algorithm>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/__algorithm/ranges/min.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/bidirectional_range.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/random_access_range.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/range_rvalue_reference_t.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/simple_view.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__tuple/apply.hpp"
#include "vccc/__tuple/tuple_fold.hpp"
#include "vccc/__tuple/tuple_for_each.hpp"
#include "vccc/__tuple/tuple_transform.hpp"
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/common_type.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/maybe_const.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"
#include "vccc/__utility/in_place.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<bool AllForward /* true */>
struct zip_view_iterator_category {
  using iterator_category = input_iterator_tag;
};

template<>
struct zip_view_iterator_category<false> {
#if __cplusplus < 202002L
  using iterator_category = iterator_ignore;
#endif
};

template<typename R>
struct min_tuple_distance_fn {
  template<typename T1, typename T2>
  R operator()(const T1& t1, const T2& t2) const {
    static_assert(std::tuple_size<remove_cvref_t<T1>>::value ==
                  std::tuple_size<remove_cvref_t<T2>>::value, "Size doesn't match");

    return call(t1, t2, in_place_index_t<0>{});
  }

 private:
  template<typename T1, typename T2, std::size_t... I>
  R call(const T1& t1, const T2& t2, std::index_sequence<I...>) const {
    return ranges::min({R(std::get<I>(t1) - std::get<I>(t2))...});
  }
};

template<typename... Rs>
struct zip_is_common
    : disjunction<
        conjunction<
            bool_constant<sizeof...(Rs) == 1>,
            common_range<Rs>...
        >,
        conjunction<
            negation<
                conjunction<bidirectional_range<Rs>...>
            >,
            common_range<Rs>...
        >,
        conjunction<
            random_access_range<Rs>...,
            sized_range<Rs>...
        >
    > {};

} // namespace detail

template<typename... Views>
class zip_view : public view_interface<zip_view<Views...>> {
  static constexpr std::size_t kViewCount = sizeof...(Views);
  using tuple_index_sequence = std::index_sequence_for<Views...>;

 public:
  static_assert(sizeof...(Views) > 0, "Constraints not satisfied");
  static_assert(conjunction<input_range<Views>...>::value, "Constraints not satisfied");
  static_assert(conjunction<view<Views>...>::value, "Constraints not satisfied");

  template<bool Const> class iterator;
  template<bool Const> class sentinel;


  template<bool Const>
  class iterator : public detail::zip_view_iterator_category<conjunction<forward_range<maybe_const<Const, Views>>...>::value> {
    using all_forward = conjunction<forward_range<maybe_const<Const, Views>>...>;
    using all_bidirectional = conjunction<bidirectional_range<maybe_const<Const, Views>>...>;
    using all_random_access = conjunction<random_access_range<maybe_const<Const, Views>>...>;

    friend class iterator<true>;
    friend class sentinel<true>;
    friend class sentinel<false>;
    friend class zip_view;

    struct tuple_deref_fn {
      template<typename I>
      constexpr decltype(auto) operator()(I& i) const {
        return *i;
      }
    };

   public:
    using iterator_concept =
        std::conditional_t<
            all_random_access::value, random_access_iterator_tag,
        std::conditional_t<
            all_bidirectional::value, bidirectional_iterator_tag,
        std::conditional_t<
            all_forward::value, forward_iterator_tag,
            input_iterator_tag
        >>>;
    using value_type = std::tuple<maybe_const<Const, Views>...>;
    using difference_type = common_type_t<range_difference_t<maybe_const<Const, Views>>...>;

#if __cplusplus < 202002L
    using pointer = void;
    using reference = decltype(vccc::tuple_transform(
            std::declval<std::tuple<iterator_t<maybe_const<Const, Views>>...>&>(),
            std::declval<tuple_deref_fn>()
        ));
#endif

    iterator() = default;

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<iterator_t<Views>, iterator_t<maybe_const<Const, Views>>>...
    >::value, int> = 0>
    constexpr iterator(iterator<AntiConst> i)
        : current_(std::move(i.current_)) {}

    constexpr decltype(auto) operator*() const {
      return vccc::tuple_transform(current_, [](auto& i) -> decltype(auto) { return *i; });
    }


    template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
        all_random_access
    >::value, int> = 0>
    constexpr decltype(auto) operator[](difference_type n) const {
      return vccc::tuple_transform(
          current_,
          [&](auto& i) -> decltype(auto) {
            static_assert(std::is_lvalue_reference<decltype(i)>::value, "Invalid type");
            using I = remove_cvref_t<decltype(i)>;
            return i[iter_difference_t<I>(n)];
          }
      );
    }

    constexpr iterator& operator++() {
      vccc::tuple_for_each(current_, [](auto& i) { ++i; });
      return *this;
    }

    template<typename AF = all_forward, std::enable_if_t<!AF::value, int> = 0>
    constexpr void operator++(int) {
      ++*this;
    }

    template<typename AF = all_forward, std::enable_if_t<AF::value, int> = 0>
    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    template<typename AB = all_bidirectional, std::enable_if_t<AB::value, int> = 0>
    constexpr iterator& operator--() {
      vccc::tuple_for_each(current_, [](auto& i) { --i; });
    }

    template<typename AB = all_bidirectional, std::enable_if_t<AB::value, int> = 0>
    constexpr iterator operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    constexpr iterator& operator+=(difference_type n) {
      vccc::tuple_for_each(current_, [&](auto& i) {
        static_assert(std::is_lvalue_reference<decltype(i)>::value, "Invalid type");
        using I = remove_cvref_t<decltype(i)>;
        i += iter_difference_t<I>(n);
      });
      return *this;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    constexpr iterator& operator-=(difference_type n) {
      vccc::tuple_for_each(current_, [&](auto& i) {
        static_assert(std::is_lvalue_reference<decltype(i)>::value, "Invalid type");
        using I = remove_cvref_t<decltype(i)>;
        i -= iter_difference_t<I>(n);
      });
      return *this;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
        equality_comparable<iterator_t<maybe_const<Const, Views>>>...
    >::value, int> = 0>
    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return x.equal(y);
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
        equality_comparable<iterator_t<maybe_const<Const, Views>>>...
    >::value, int> = 0>
    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr bool operator<(const iterator& x, const iterator& y) {
      return x.current_ < y.current_;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr bool operator<=(const iterator& x, const iterator& y) {
      using namespace rel_ops;
      return x.current_ <= y.current_;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr bool operator>(const iterator& x, const iterator& y) {
      using namespace rel_ops;
      return x.current_ > y.current_;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr bool operator>=(const iterator& x, const iterator& y) {
      using namespace rel_ops;
      return x.current_ >= y.current_;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr iterator operator+(const iterator& i, difference_type n) {
      auto r = i;
      r += n;
      return r;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr iterator operator+(difference_type n, const iterator& i) {
      auto r = i;
      r += n;
      return r;
    }

    template<typename AR = all_random_access, std::enable_if_t<AR::value, int> = 0>
    friend constexpr iterator operator-(const iterator& i, difference_type n) {
      auto r = i;
      r -= n;
      return r;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
        sized_sentinel_for<iterator_t<maybe_const<Const, Views>>,
                           iterator_t<maybe_const<Const, Views>>>
        ...
    >::value, int> = 0>
    friend constexpr difference_type operator-(const iterator& i, const iterator& j) {
      return detail::min_tuple_distance_fn<difference_type>{}(i.current_, j.current_);
    }

    friend constexpr auto iter_move(const iterator& i)
        noexcept(conjunction<
            bool_constant<noexcept(
                ranges::iter_move(
                    std::declval<const iterator_t<maybe_const<Const, Views>>&>()
                )
            )>...,
            std::is_nothrow_move_constructible<
                range_rvalue_reference_t<maybe_const<Const, Views>>
            >...
        >::value)
    {
      return vccc::tuple_transform(i.current_, ranges::iter_move);
    }

    // iter_swap

   private:
    template<typename Tuple>
    constexpr explicit iterator(in_place_t, Tuple&& tup)
        : current_(std::forward<Tuple>(tup)) {}

    constexpr bool equal(const iterator& other) const {
      return equal_1(other, all_bidirectional{});
    }

    constexpr bool equal_1(const iterator& other, std::true_type /* all_bidirectional */) const {
      return current_ == other.current_;
    }

    constexpr bool equal_1(const iterator& other, std::false_type /* all_bidirectional */) const {
      return equal_2(other, in_place_index_t<0>{});
    }

    template<typename OtherTuple, std::size_t I, std::enable_if_t<I != kViewCount, int> = 0>
    constexpr bool equal_2(const OtherTuple& t, in_place_index_t<I>) const {
      return bool(std::get<I>(current_) == std::get<I>(t))
          ? true
          : equal_2(t, in_place_index_t<I + 1>{});
    }

    template<typename OtherTuple>
    constexpr bool equal_2(const OtherTuple& other, in_place_index_t<kViewCount>) const {
      return false;
    }

    std::tuple<iterator_t<maybe_const<Const, Views>>...> current_{};
  };

  template<bool Const>
  class sentinel {
    friend class zip_view;

   public:
    sentinel() = default;

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<sentinel_t<Views>, sentinel_t<maybe_const<Const, Views>>>...
    >::value, int> = 0>
    constexpr sentinel(sentinel<AntiConst> i)
        : end_(std::move(i.end_)) {}

    template<bool OtherConst, std::enable_if_t<conjunction<
        sentinel_for<
            sentinel_t<maybe_const<Const, Views>>,
            iterator_t<maybe_const<OtherConst, Views>>
        >...
    >::value, int> = 0>
    friend constexpr bool operator==(const iterator<OtherConst>& x, const sentinel& y) {
      return x.equal_2(y.end_, in_place_index_t<0>{});
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sentinel_for<
            sentinel_t<maybe_const<Const, Views>>,
            iterator_t<maybe_const<OtherConst, Views>>
        >...
    >::value, int> = 0>
    friend constexpr bool operator==(const sentinel& y, const iterator<OtherConst>& x) {
      return x == y;
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sentinel_for<
            sentinel_t<maybe_const<Const, Views>>,
            iterator_t<maybe_const<OtherConst, Views>>
        >...
    >::value, int> = 0>
    friend constexpr bool operator!=(const iterator<OtherConst>& x, const sentinel& y) {
      return !(x == y);
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sentinel_for<
            sentinel_t<maybe_const<Const, Views>>,
            iterator_t<maybe_const<OtherConst, Views>>
        >...
    >::value, int> = 0>
    friend constexpr bool operator!=(const sentinel& y, const iterator<OtherConst>& x) {
      return !(x == y);
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sentinel_for<
            sentinel_t<maybe_const<Const, Views>>,
            iterator_t<maybe_const<OtherConst, Views>>
        >...
    >::value, int> = 0>
    friend constexpr common_type_t<range_difference_t<maybe_const<OtherConst, Views>>...>
    operator-(const iterator<OtherConst>& x, const sentinel& y) {
      using R = common_type_t<range_difference_t<maybe_const<OtherConst, Views>>...>;
      return detail::min_tuple_distance_fn<R>{}(x.current_, y.end_);
    }

    template<bool OtherConst, std::enable_if_t<conjunction<
        sentinel_for<
            sentinel_t<maybe_const<Const, Views>>,
            iterator_t<maybe_const<OtherConst, Views>>
        >...
    >::value, int> = 0>
    friend constexpr common_type_t<range_difference_t<maybe_const<OtherConst, Views>>...>
    operator-(const sentinel& y, const iterator<OtherConst>& x) {
      return -(x - y);
    }

   private:
    template<typename Tuple>
    constexpr explicit sentinel(in_place_t, Tuple&& tup)
        : end_(std::forward<Tuple>(tup)) {}

    std::tuple<sentinel_t<maybe_const<Const, Views>>...> end_{};
  };

  zip_view() = default;

  constexpr zip_view(Views... views)
      : views_(std::move(views)...) {}

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      negation<
          simple_view<Views>
      >...
  >::value, int> = 0>
  constexpr auto begin() {
    return iterator<false>(in_place, vccc::tuple_transform(views_, ranges::begin));
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      range<const Views>...
  >::value, int> = 0>
  constexpr auto begin() const {
    return iterator<true>(in_place, vccc::tuple_transform(views_, ranges::begin));
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      negation<
          simple_view<Views>
      >...
  >::value, int> = 0>
  constexpr auto end() {
    return end_impl<false>(*this, detail::zip_is_common<Views...>{}, conjunction<random_access_range<Views>...>{});
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      range<const Views>...
  >::value, int> = 0>
  constexpr auto end() const {
    return end_impl<true>(*this, detail::zip_is_common<const Views...>{}, conjunction<random_access_range<const Views>...>{});
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      sized_range<Views>...
  >::value, int> = 0>
  constexpr auto size() {
    return vccc::apply(
        [](auto... sizes) {
          using CT = std::make_unsigned_t<common_type_t<decltype(sizes)...>>;
          return ranges::min({CT(sizes)...});
        },
        vccc::tuple_transform(views_, ranges::size)
    );
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<std::is_void<Dummy>,
      sized_range<const Views>...
  >::value, int> = 0>
  constexpr auto size() const {
    return vccc::apply(
        [](auto... sizes) {
          using CT = std::make_unsigned_t<common_type_t<decltype(sizes)...>>;
          return ranges::min({CT(sizes)...});
        },
        vccc::tuple_transform(views_, ranges::size)
    );
  }

 private:
  template<bool Const, typename Self, typename Any>
  static constexpr auto end_impl(Self&& self, std::false_type /* zip_is_common */, Any) {
    return sentinel<Const>{in_place, vccc::tuple_transform(self.views_, ranges::end)};
  }

  template<bool Const, typename Self>
  static constexpr auto end_impl(Self&& self, std::true_type /* zip_is_common */, std::true_type /* random_access_range */) {
    return self.begin() + iter_difference_t<iterator<Const>>(self.size());
  }

  template<bool Const, typename Self>
  static constexpr auto end_impl(Self&& self, std::true_type /* zip_is_common */, std::false_type /* random_access_range */) {
    return iterator<Const>(in_place, vccc::tuple_transform(self.views_, ranges::end));
  }

  std::tuple<Views...> views_{};
};

#if __cplusplus >= 201703L

template<typename... Rs>
zip_view(Rs&&...) -> zip_view<views::all_t<Rs>...>;

#endif

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ZIP_VIEW_HPP_
