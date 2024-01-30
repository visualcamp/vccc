//
// Created by yonggyulee on 1/31/24.
//

#ifndef VCCC_RANGES_VIEWS_JOIN_VIEW_HPP_
#define VCCC_RANGES_VIEWS_JOIN_VIEW_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__concepts/copyable.hpp"
#include "vccc/__concepts/default_initializable.hpp"
#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__ranges/bidirectional_range.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/detail/simple_view.hpp"
#include "vccc/__ranges/forward_range.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/non_propagating_cache.hpp"
#include "vccc/__ranges/range_difference_t.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/maybe_const.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/common_type.hpp"
#include "vccc/__type_traits/has_operator_arrow.hpp"
#include "vccc/__utility/as_const.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename Base>
using join_view_iterator_concept =
    std::conditional_t<
        conjunction<
            std::is_reference<range_reference_t<Base>>,
            bidirectional_range<Base>,
            bidirectional_range<range_reference_t<Base>>
        >::value,
        bidirectional_iterator_tag,
    std::conditional_t<
        conjunction<
            std::is_reference<range_reference_t<Base>>,
            forward_range<Base>,
            forward_range<range_reference_t<Base>>
        >::value,
        forward_iterator_tag,
        input_iterator_tag
    >>;

template<typename ThisC, typename OuterC, typename InnerC>
struct join_view_iterator_category_impl {
#if __cplusplus
  using iterator_category = iterator_ignore;
#endif
};

template<typename OuterC, typename InnerC>
struct join_view_iterator_category_impl<forward_iterator_tag, OuterC, InnerC> {
  using iterator_category =
      std::conditional_t<
          conjunction<
              derived_from<OuterC, bidirectional_iterator_tag>,
              derived_from<InnerC, bidirectional_iterator_tag>
          >::value,
          bidirectional_iterator_tag,
      std::conditional_t<
          conjunction<
              derived_from<OuterC, forward_iterator_tag>,
              derived_from<InnerC, forward_iterator_tag>
          >::value,
          forward_iterator_tag,
          input_iterator_tag
      >>;
};

template<typename Base>
struct join_view_iterator_category
    : join_view_iterator_category_impl<
          join_view_iterator_concept<Base>,
          typename cxx20_iterator_traits<iterator_t<Base>>::iterator_category,
          typename cxx20_iterator_traits<iterator_t<range_reference_t<Base>>>::iterator_category
      > {};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename V>
class join_view : public view_interface<join_view<V>> {
 public:
  static_assert(input_range<V>::value, "Constraints not satisfied");
  static_assert(view<V>::value, "Constraints not satisfied");
  static_assert(input_range<range_reference_t<V>>::value, "Constraints not satisfied");

 private:
  template <bool Const>
  using InnerRng = range_reference_t<detail::maybe_const<Const, V>>;

 public:
  template<bool Const> class iterator;
  template<bool Const> friend class iterator;
  template<bool Const> class sentinel;

  template<bool Const>
  class iterator : detail::join_view_iterator_category<detail::maybe_const<Const, V>> {
    using Parent = detail::maybe_const<Const, join_view>;
    using Base = detail::maybe_const<Const, V>;
    using OuterIter = iterator_t<Base>;
    using InnerIter = iterator_t<range_reference_t<Base>>;
    static constexpr bool ref_is_glvalue = std::is_reference<range_reference_t<Base>>::value;

    friend class join_view;
    template<bool> friend class sentinel;

   public:
    using iterator_concept = detail::join_view_iterator_concept<Base>;

    using value_type = range_value_t<range_reference_t<Base>>;
    using difference_type = common_type_t<range_difference_t<Base>, range_difference_t<range_reference_t<Base>>>;
#if __cplusplus < 202002L
    using pointer = void;
    using reference = void;
#endif

    template<typename O = OuterIter, std::enable_if_t<conjunction<
        default_initializable<O>,
        default_initializable<InnerIter>
    >::value, int> = 0>
    iterator()
        : outer_(), inner_(), parent_(nullptr) {}

    constexpr iterator(Parent& parent, OuterIter outer)
        : outer_(std::move(outer)), parent_(vccc::addressof(parent))
    {
      satisfy();
    }

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<iterator_t<V>, OuterIter>,
        convertible_to<iterator_t<InnerRng<false>>, InnerIter>
    >::value, int> = 0>
    constexpr iterator(iterator<AntiConst> i)
        : outer_(std::move(i.outer_)), inner_(std::move(i.inner_)), parent_(i.parent_) {}

    constexpr decltype(auto) operator*() const {
      return *inner_;
    }

    template<typename II = InnerIter, std::enable_if_t<conjunction<
        has_operator_arrow<II>,
        copyable<II>
    >::value, int> = 0>
    constexpr decltype(auto) operator->() const {
      return inner_;
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<B, int> = 0>
    constexpr iterator& operator++() {
      auto&& inner_rng = *outer_;
      if (++inner_ == ranges::end(inner_rng)) {
        ++outer_;
        satisfy();
      }
      return *this;
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<B == false, int> = 0>
    constexpr iterator& operator++() {
      auto&& inner_rng = *parent_->inner_;
      if (++inner_ == ranges::end(inner_rng)) {
        ++outer_;
        satisfy();
      }
      return *this;
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<conjunction<
        bool_constant<B>,
        forward_range<Base>,
        forward_range<range_reference_t<Base>>
    >::value == false, int> = 0>
    constexpr void operator++() {
      ++*this;
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<conjunction<
        bool_constant<B>,
        forward_range<Base>,
        forward_range<range_reference_t<Base>>
    >::value, int> = 0>
    constexpr iterator operator++() {
      auto tmp = *this;
      ++*this;
      return tmp;
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<conjunction<
        bool_constant<B>,
        bidirectional_range<Base>,
        bidirectional_range<range_reference_t<Base>>,
        common_range<range_reference_t<Base>>
    >::value, int> = 0>
    constexpr iterator& operator--() {
      if (outer_ == ranges::end(parent_->base_))
        inner_ = ranges::end(*--outer_);
      while (inner_ == ranges::begin(*outer_))
        inner_ = ranges::end(*--outer_);
      --inner_;
      return *this;
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<conjunction<
        bool_constant<B>,
        bidirectional_range<Base>,
        bidirectional_range<range_reference_t<Base>>,
        common_range<range_reference_t<Base>>
    >::value, int> = 0>
    constexpr iterator operator--(int) {
      auto tmp = *this;
      --*this;
      return tmp;
    }

    constexpr void satisfy() {
      satisfy_impl(bool_constant<ref_is_glvalue>{});
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<conjunction<
        bool_constant<B>,
        equality_comparable<iterator_t<Base>>,
        equality_comparable<iterator_t<range_reference_t<Base>>>
    >::value, int> = 0>
    friend constexpr bool operator==(const iterator& x, const iterator& y) {
      return (x.outer_ == y.outer_) && (x.inner_ == y.inner_);
    }

    template<bool B = ref_is_glvalue, std::enable_if_t<conjunction<
        bool_constant<B>,
        equality_comparable<iterator_t<Base>>,
        equality_comparable<iterator_t<range_reference_t<Base>>>
    >::value, int> = 0>
    friend constexpr bool operator!=(const iterator& x, const iterator& y) {
      return !(x == y);
    }

    friend constexpr decltype(auto) iter_move(const iterator& i)
        noexcept(noexcept(ranges::iter_move(*i.inner_)))
    {
      return ranges::iter_move(*i.inner_);
    }

    // TODO: Implement iter_swap

   private:
    constexpr OuterIter& get_outer() noexcept {
      return get_outer(forward_range<Base>{});
    }
    constexpr const OuterIter& get_outer() const noexcept {
      return get_outer(forward_range<Base>{});
    }
    constexpr OuterIter& get_outer_impl(std::true_type /* forward_range */) noexcept {
        return outer_;
    }
    constexpr OuterIter& get_outer_impl(std::false_type /* forward_range */) noexcept {
        return *parent_->outer_;
    }
    constexpr OuterIter& get_outer_impl(std::true_type /* forward_range */) const noexcept {
        return outer_;
    }
    constexpr OuterIter& get_outer_impl(std::false_type /* forward_range */) const noexcept {
        return *parent_->outer_;
    }

    constexpr void satisfy_impl(std::true_type /* ref_is_glvalue */) {
      for (; outer_ != ranges::end(parent_->base_); ++outer_) {
        auto&& inner = *vccc::as_const(outer_);
        inner_ = ranges::begin(inner);
        if (inner_ != ranges::end(inner))
          return;
      }

      inner_ = InnerIter();
    }
    constexpr void satisfy_impl(std::false_type /* ref_is_glvalue */) {
      for (; outer_ != ranges::end(parent_->base_); ++outer_) {
        auto&& inner = parent_->inner_.emplace_deref(outer_);
        inner_ = ranges::begin(inner);
        if (inner_ != ranges::end(inner))
          return;
      }
    }

    OuterIter outer_;
    non_propagating_cache<InnerIter> inner_;
    Parent* parent_;
  };

  template<bool Const>
  class sentinel {
    using Parent = detail::maybe_const<Const, join_view>;
    using Base = detail::maybe_const<Const, V>;

   public:
    sentinel() = default;

    constexpr explicit sentinel(Parent& parent)
        : end_(ranges::end(parent.base_)) {}

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<sentinel_t<V>, sentinel_t<Base>>
    >::value, int> = 0>
    constexpr sentinel(sentinel<AntiConst> s)
        : end_(std::move(s.end_)) {}

    friend constexpr bool operator==(const iterator<Const>& x, const sentinel& y) {
      using namespace vccc::rel_ops;
      return x.get_outer() == y.end_;
    }

    friend constexpr bool operator==(const sentinel& y, const iterator<Const>& x) {
      return x == y;
    }

   private:
    sentinel_t<Base> end_;
  };

  template<typename V2 = V, std::enable_if_t<default_initializable<V2>::value, int> = 0>
  join_view() : base_(V()) {}

  constexpr explicit join_view(V base) : base_(std::move(base)) {}

  template<typename V2 = V, std::enable_if_t<copy_constructible<V2>::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  constexpr auto begin() {
    using B = conjunction<detail::simple_view<V>, std::is_reference<range_reference_t<V>>>;
    return iterator<B::value>{*this, ranges::begin(base_)};
  }

  template<typename V2 = V, std::enable_if_t<conjunction<
      input_range<const V2>,
      std::is_reference<range_reference_t<const V2>>
  >::value, int> = 0>
  constexpr auto begin() const {
    return iterator<true>{*this, ranges::begin(base_)};
  }

  constexpr auto end() {
    return end_impl(conjunction<
        forward_range<V>,
        std::is_reference<range_reference_t<V>>,
        forward_range<range_reference_t<V>>,
        common_range<V>,
        common_range<range_reference_t<V>>
      >{});
  }

  template<typename V2 = V, std::enable_if_t<conjunction<
      input_range<const V2>,
      std::is_reference<range_reference_t<const V2>>
  >::value, int> = 0>
  constexpr auto end() const {
    return end_impl(conjunction<
        forward_range<const V>,
        std::is_reference<range_reference_t<const V>>,
        forward_range<range_reference_t<const V>>,
        common_range<const V>,
        common_range<range_reference_t<const V>>
      >{});
  }

 private:
  constexpr auto end_impl(std::true_type) {
    return iterator<detail::simple_view<V>::value>{*this, ranges::end(base_)};
  }
  constexpr auto end_impl(std::false_type) {
    return sentinel<detail::simple_view<V>::value>{*this};
  }

  constexpr auto end_impl(std::true_type) const {
    return iterator<true>{*this, ranges::end(base_)};
  }
  constexpr auto end_impl(std::false_type) const {
    return sentinel<true>{*this};
  }

  V base_;
};

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_JOIN_VIEW_HPP_
