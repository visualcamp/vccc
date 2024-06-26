//
// Created by YongGyu Lee on 2/20/24.
//

#ifndef CONCAT_WITH_VIEW_HPP
#define CONCAT_WITH_VIEW_HPP

#include <functional>
#include <tuple>
#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/next.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/simple_view.hpp"
#include "vccc/__ranges/distance.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__tuple/tuple_fold.hpp"
#include "vccc/__tuple/tuple_transform.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/common_type.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/maybe_const.hpp"
#include "vccc/__utility/type_sequence.hpp"
#include "vccc/variant.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename Pattern, typename... Rngs>
using concat_with_compatible = conjunction<
    has_typename_type<common_type<range_value_t<Pattern>, range_value_t<Rngs>...>>,
    has_typename_type<common_reference<range_reference_t<Pattern>, range_reference_t<Rngs>...>>,
    has_typename_type<common_reference<range_rvalue_reference_t<Pattern>, range_rvalue_reference_t<Rngs>...>>
>;

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename Pattern, typename... Rngs>
struct concat_with_view : view_interface<concat_with_view<Pattern, Rngs...>> {
  static_assert(sizeof...(Rngs) != 0, "Constraints not satisfied");
  static_assert(conjunction<input_range<Rngs>...>::value, "Constraints not satisfied");
  static_assert(view<Pattern>::value, "Constraints not satisfied");
  static_assert(detail::concat_with_compatible<Pattern, Rngs...>::value, "Constraints not satisfied");

 private:
  using difference_type_ = common_type_t<range_difference_t<Rngs>...>;
  using BackBase = typename type_sequence<Rngs...>::back;

  static constexpr std::size_t cranges = sizeof...(Rngs);
  std::tuple<Rngs...> bases_{};
  Pattern pattern_;

 public:
  template<bool IsConst>
  struct iterator;

  template<bool Const>
  struct sentinel {
   private:
    friend struct sentinel<!Const>;
    friend struct iterator<Const>;
    friend class concat_with_view;

    using Parent = maybe_const<Const, concat_with_view>;
    using Base = maybe_const<Const, BackBase>;
    sentinel_t<Base> end_{};

    constexpr sentinel(Parent& parent)
        : end_(ranges::end(std::get<cranges - 1>(parent.bases_))) {}

   public:
    sentinel() = default;

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<sentinel_t<BackBase>, sentinel_t<Base>>
    >::value, int> = 0>
    constexpr sentinel(sentinel<AntiConst> that)
        : end_(std::move(that.end_)) {}
  };

  template<bool IsConst>
  struct iterator {
    using iterator_category = input_iterator_tag;
    using iterator_concept =
        std::conditional_t<
            conjunction<random_access_range<Rngs>...>::value, random_access_iterator_tag,
        std::conditional_t<
            conjunction<bidirectional_range<Rngs>...>::value, bidirectional_iterator_tag,
        std::conditional_t<
            conjunction<forward_range<Rngs>...>::value, forward_iterator_tag,
            input_iterator_tag
        >>>;
    using value_type = common_type_t<range_value_t<maybe_const<IsConst, Rngs>>...>;
    using difference_type = common_type_t<range_difference_t<Rngs>...>;
    using reference = common_reference_t<range_reference_t<maybe_const<IsConst, Rngs>>...>;
    using pointer = void;

   private:
    friend struct iterator<!IsConst>;
    using Parent = maybe_const<IsConst, concat_view>;
    Parent* parent_;
    variant<iterator_t<maybe_const<IsConst, Rngs>>...> its_;

    template<std::size_t N, std::enable_if_t<(N < cranges - 1), int> = 0>
    void satisfy(std::integral_constant<std::size_t, N>) {
      if (std::get<N>(its_) == ranges::end(std::get<N>(parent_->bases_))) {
        its_.template emplace<N + 1>(ranges::begin(std::get<N + 1>(parent_->bases_)));
        this->satisfy(std::integral_constant<size_t, N + 1>{});
      }
    }

    template<std::size_t N, std::enable_if_t<(N >= cranges - 1), int> = 0>
    void satisfy(std::integral_constant<std::size_t, N>) { /* no op */ }

    struct next_raw_visitor {
      iterator* pos;

      template<typename I, std::size_t N>
      void operator()(I& it, in_place_index_t<N>) {
        ++it;
        pos->satisfy(std::integral_constant<size_t, N>{});
      }

      template<typename U> void operator()(U&, in_place_index_t<variant_npos>) { /* no op */ }
    };

    struct prev_raw_visitor {
      iterator* pos;

      template<typename I>
      void operator()(I& it, in_place_index_t<0>) {
        --it;
      }

      template<typename I, std::size_t N, std::enable_if_t<conjunction<
          bool_constant<(N != 0)>,
          bidirectional_iterator<I>
      >::value, int> = 0>
      void operator()(I& it, in_place_index_t<N>) {
        if (it == ranges::begin(std::get<N>(pos->parent_->bases_))) {
          auto&& rng = std::get<N - 1>(pos->parent_->bases_);

          pos->its_.template emplace<N - 1>(ranges::next(ranges::begin(rng), ranges::end(rng)));
          vccc::detail::variant_raw_visit(pos->its_.index(), pos->its_._base().storage(), *this);
        } else {
          --it;
        }
      }

      template<typename U> void operator()(U&, in_place_index_t<variant_npos>) { /* no op */ }
    };

    struct advance_fwd_raw_visitor {
      iterator* pos;
      difference_type n;

      template<typename I, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
      void operator()(I& it, in_place_index_t<cranges - 1>) {
        ranges::advance(it, n);
      }

      template<typename I, std::size_t N, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
      void operator()(I& it, in_place_index_t<N>) {
        auto last = ranges::end(std::get<N>(pos->parent_->bases_));
        // BUGBUG If distance(it, last) > n, then using bounded advance
        // is O(n) when it need not be since the last iterator position
        // is actually not interesting. Only the "rest" is needed, which
        // can sometimes be O(1).
        auto rest = ranges::advance(it, n, std::move(last));
        pos->satisfy(std::integral_constant<size_t, N>{});
        if (rest != 0) {
          vccc::detail::variant_raw_visit(
              pos->its_.index(), pos->its_._base().storage(), advance_fwd_raw_visitor{pos, rest});
        }
      }

      template<typename U> void operator()(U&, in_place_index_t<variant_npos>) { /* no op */ }
    };

    struct advance_rev_raw_visitor {
      iterator* pos;
      difference_type n;

      template<typename I, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
      void operator()(I& it, in_place_index_t<0>) {
        ranges::advance(it, n);
      }

      template<typename I, std::size_t N, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
      void operator()(I& it, in_place_index_t<N>) {
        auto first = ranges::begin(std::get<N>(pos->parent_->bases_));
        if (it == first) {
          auto&& rng = std::get<N - 1>(pos->parent_->bases_);
          pos->its_.template emplace<N - 1>(ranges::next(ranges::begin(rng), ranges::end(rng)));
          vccc::detail::variant_raw_visit(pos->its_.index(), pos->its_._base().storage(), *this);
        } else {
          auto rest = ranges::advance(it, n, std::move(first));
          if (rest != 0) {
            vccc::detail::variant_raw_visit(
                pos->its_.index(), pos->its_._base().storage(), advance_rev_raw_visitor{pos, rest});
          }
        }
      }

      template<typename U> void operator()(U&, in_place_index_t<variant_npos>) { /* no op */ }
    };

    static difference_type distance_to_(std::integral_constant<size_t, cranges>, iterator const &, iterator const &) {
      // unreachable
      return -1;
    }

    template<std::size_t N>
    static difference_type distance_to_(std::integral_constant<size_t, N>, const iterator& from, const iterator& to) {
      if (from.its_.index() > N)
        return iterator::distance_to_(std::integral_constant<size_t, N + 1>{}, from, to);

      if (from.its_.index() == N) {
        if (to.its_.index() == N)
          return ranges::distance(vccc::detail::variant_raw_get(from.its_._base().storage(), in_place_index<N>),
                                  vccc::detail::variant_raw_get(to.its_._base().storage(), in_place_index<N>));
        return ranges::distance(vccc::detail::variant_raw_get(from.its_._base().storage(), in_place_index<N>),
                                ranges::end(std::get<N>(from.parent_->bases_)))
               + iterator::distance_to_(std::integral_constant<size_t, N + 1>{}, from, to);
      }

      if (from.its_.index() < N && to.its_.index() > N)
          return ranges::distance(std::get<N>(from.parent_->bases_))
              + iterator::distance_to_(std::integral_constant<size_t, N + 1>{}, from, to);

      return ranges::distance(ranges::begin(std::get<N>(from.parent_->bases_)),
                              vccc::detail::variant_raw_get(to.its_._base().storage(), in_place_index<N>));
    }

   public:
    iterator() = default;

    template<std::size_t I, typename It>
    constexpr iterator(Parent* parent, in_place_index_t<I>, It&& it)
      : parent_(parent)
      , its_{in_place_index<I>, std::forward<It>(it)} // ranges::begin(std::get<0>(parent->bases_))
    {
        this->satisfy(std::integral_constant<size_t, I>{});
    }

    // iterator(Parent* parent, end_tag)
    //   : parent_(parent)
    //   , its_{in_place_index<cranges - 1>, ranges::end(std::get<cranges - 1>(parent->bases_))} {}


    template<bool Other, std::enable_if_t<conjunction<
        bool_constant<(Other != IsConst && IsConst)>
    >::value, int> = 0>
    constexpr iterator(iterator<Other> that)
      : parent_(that.parent_)
      , its_(std::move(that.its_)) {}

    constexpr decltype(auto) operator*() const {
      return its_.visit([](auto&& it) -> iterator::reference { return *it; });
    }

    template<typename V = variant<iterator_t<maybe_const<IsConst, Rngs>>...>, std::enable_if_t<
        equality_comparable<V>
    ::value, int> = 0>
    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return x.its_ == y.its_;
    }

    template<typename V = variant<iterator_t<maybe_const<IsConst, Rngs>>...>, std::enable_if_t<
        equality_comparable<V>
    ::value, int> = 0>
    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

    friend constexpr bool operator==(const iterator& x, const sentinel<IsConst>& y) {
      return x.equal(y);
    }

    friend constexpr bool operator!=(const iterator& x, const sentinel<IsConst>& y) {
      return !(x == y);
    }

    friend constexpr bool operator==(const sentinel<IsConst>& y, const iterator& x) {
      return x == y;
    }

    friend constexpr bool operator!=(const sentinel<IsConst>& y, const iterator& x) {
      return !(x == y);
    }

    constexpr bool equal(const sentinel<IsConst>& pos) const {
      return its_.index() == cranges - 1 && std::get<cranges - 1>(its_) == pos.end_;
    }

    constexpr iterator& operator++() {
      vccc::detail::variant_raw_visit(its_.index(), its_._base().storage(), next_raw_visitor{this});
      return *this;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        negation<forward_range<Rngs>>...
    >::value, int> = 0>
    constexpr void operator++(int) {
      ++*this;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        forward_range<Rngs>...
    >::value, int> = 0>
    constexpr iterator operator++(int) {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        bidirectional_range<Rngs>...
    >::value, int> = 0>
    constexpr iterator& operator--() {
      vccc::detail::variant_raw_visit(its_.index(), its_._base().storage(), prev_raw_visitor{this});
      return *this;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        bidirectional_range<Rngs>...
    >::value, int> = 0>
    constexpr iterator& operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        random_access_range<Rngs>...
    >::value, int> = 0>
    constexpr iterator& operator+=(difference_type n) {
      if (n > 0) {
        vccc::detail::variant_raw_visit(its_.index(), its_._base().storage(), advance_fwd_raw_visitor{this, n});
      } else if (n < 0) {
        vccc::detail::variant_raw_visit(its_.index(), its_._base().storage(), advance_rev_raw_visitor{this, n});
      }
      return *this;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        random_access_range<Rngs>...
    >::value, int> = 0>
    constexpr iterator& operator-=(difference_type n) {
      *this += -n;
      return *this;
    }

    template<typename Dummy = void, std::enable_if_t<conjunction<
        std::is_void<Dummy>,
        sized_sentinel_for<iterator_t<Rngs>, iterator_t<Rngs>>...
    >::value, int> = 0>
    friend constexpr difference_type operator-(const iterator& x, const iterator& y) {
      if (x.its_.index() <= y.its_.index())
        return -iterator::distance_to_(std::integral_constant<std::size_t, 0>{}, x, y);
      return iterator::distance_to_(std::integral_constant<std::size_t, 0>{}, y, x);
    }

  };


  concat_with_view() = default;

  explicit concat_with_view(Rngs... rngs)
      : bases_{std::move(rngs)...} {}

  constexpr auto begin() {
    using Const = conjunction<simple_view<Rngs>...>;
    return iterator<Const::value>{this, in_place_index<0>, ranges::begin(std::get<0>(bases_))};
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<
      std::is_void<Dummy>,
      range<const Rngs>...
  >::value, int> = 0>
  constexpr iterator<true> begin() const {
    return iterator<true>{this, in_place_index<0>, ranges::begin(std::get<0>(bases_))};
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<
      std::is_void<Dummy>,
      common_range<Rngs>...
  >::value, int> = 0>
  constexpr auto end() {
    using Const = conjunction<simple_view<Rngs>...>;
    return iterator<Const::value>{this, in_place_index<cranges - 1>, ranges::end(std::get<cranges - 1>(bases_))};
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<
      std::is_void<Dummy>,
      negation<common_range<Rngs>>...
  >::value, int> = 0>
  constexpr auto end() {
    using Const = conjunction<simple_view<Rngs>...>;
    return sentinel<Const::value>{*this};
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<
      std::is_void<Dummy>,
      range<const Rngs>...
  >::value, int> = 0>
  constexpr auto end() const {
    return end_impl(conjunction<common_range<const Rngs>...>{});
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<
      std::is_void<Dummy>,
      sized_range<Rngs>...
  >::value, int> = 0>
  constexpr auto size() {
    return vccc::tuple_fold_left(
        vccc::tuple_transform(bases_, [](auto&& r) { return r.size(); }),
        std::size_t{0},
        std::plus<>{}
    );
  }

  template<typename Dummy = void, std::enable_if_t<conjunction<
      std::is_void<Dummy>,
      sized_range<const Rngs>...
  >::value, int> = 0>
  constexpr auto size() const {
    return vccc::tuple_fold_left(
        vccc::tuple_transform(bases_, [](auto&& r) { return r.size(); }),
        std::size_t{0},
        std::plus<>{}
    );
  }

 private:
  constexpr iterator<true> end_impl(std::true_type /* common_range */) const {
    return iterator<true>{this, in_place_index<cranges - 1>, ranges::end(std::get<cranges - 1>(bases_))};
  }

  constexpr sentinel<true> end_impl(std::false_type /* common_range */) const {
    return sentinel<true>{*this};
  }
};

#if __cplusplus >= 201703L

template<typename... Rng>
concat_view(Rng &&...) //
    -> concat_view<views::all_t<Rng>...>;

#endif

namespace views {
namespace detail {

struct concat_with_niebloid {
  template<typename... R, std::enable_if_t<conjunction<
      viewable_range<R>...,
      input_range<R>...
  >::value, int> = 0>
  constexpr auto operator()(R&&... rs) const {
    return concat_with_view<all_t<R>...>{views::all(std::forward<R>(rs))...};
  }
};

} // namespace detail

/// @brief concatenate ranges
constexpr VCCC_INLINE_OR_STATIC detail::concat_with_niebloid concat_with{};

/// @}

} // namespace views

} // namespace ranges
} // namespace vccc

#endif // CONCAT_WITH_VIEW_HPP
