//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_VIEWS_TAKE_WHILE_VIEW_HPP_
#define VCCC_RANGES_VIEWS_TAKE_WHILE_VIEW_HPP_

#include <memory>
#include <type_traits>
#include <utility>

#include "vccc/__core/constexpr.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__functional/invoke.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/movable_box.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/simple_view.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/maybe_const.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup ranges
/// @{

template<typename V, typename Pred>
class take_while_view {
 public:
  static_assert(ranges::view<V>::value, "Constraints not satisfied");
  static_assert(ranges::input_range<V>::value, "Constraints not satisfied");
  static_assert(std::is_object<Pred>::value, "Constraints not satisfied");
  static_assert(indirect_unary_predicate<const Pred, iterator_t<V>>::value, "Constraints not satisfied");

  template<bool Const>
  class sentinel {
    using Base = maybe_const<Const, V>;
    friend class sentinel<!Const>;

   public:
    sentinel() = default;

    constexpr explicit sentinel(sentinel_t<Base> end, const Pred* pred)
        : end_(std::move(end)), pred_(pred) {}

    template<bool AntiConst, std::enable_if_t<conjunction<
        bool_constant<((Const != AntiConst) && Const)>,
        convertible_to<sentinel_t<V>, sentinel_t<Base>>
    >::value, int> = 0>
    constexpr sentinel(sentinel<AntiConst> that)
        : end_(std::move(that.end_)), pred_(that.pred_) {}

    constexpr sentinel_t<Base> base() const {
      return end_;
    }

    friend constexpr bool operator==(const iterator_t<Base>& x, const sentinel& y) {
      using namespace vccc::rel_ops;
      return x == y.end_ || !vccc::invoke(*y.pred_, *x);
    }

    friend constexpr bool operator!=(const iterator_t<Base>& x, const sentinel& y) {
      return !(x == y);
    }

    friend constexpr bool operator==(const sentinel& y, const iterator_t<Base>& x) {
      return x == y;
    }

    friend constexpr bool operator!=(const sentinel& y, const iterator_t<Base>& x) {
      return !(x == y);
    }

   private:
    sentinel_t<Base> end_;
    const Pred* pred_;
  };

  take_while_view() = default;

  constexpr explicit take_while_view(V base, Pred pred)
      : base_(std::move(base)), pred_(std::move(pred)) {}

  template<typename Dummy = void, std::enable_if_t<vccc::conjunction<std::is_void<Dummy>,
      copy_constructible<V>
  >::value, int> = 0>
  constexpr V base() const& {
    return base_;
  }

  constexpr V base() && {
    return std::move(base_);
  }

  constexpr const Pred& pred() const {
    return *pred_;
  }

  template<typename Dummy = void, std::enable_if_t<vccc::conjunction<std::is_void<Dummy>,
      negation<simple_view<V>>
  >::value, int> = 0>
  constexpr auto begin() {
    return ranges::begin(base_);
  }

  template<typename Dummy = void, std::enable_if_t<vccc::conjunction<std::is_void<Dummy>,
      range<const V>,
      indirect_unary_predicate<const Pred, iterator_t<const V>>
  >::value, int> = 0>
  constexpr auto begin() const {
    return ranges::begin(base_);
  }

  template<typename Dummy = void, std::enable_if_t<vccc::conjunction<std::is_void<Dummy>,
      negation<simple_view<V>>
  >::value, int> = 0>
  VCCC_CONSTEXPR_AFTER_CXX17 auto end() {
    return sentinel<false>{ranges::end(base_), pred_.operator->()};
  }

  template<typename Dummy = void, std::enable_if_t<vccc::conjunction<std::is_void<Dummy>,
      range<const V>,
      indirect_unary_predicate<const Pred, iterator_t<const V>>
  >::value, int> = 0>
  VCCC_CONSTEXPR_AFTER_CXX17 auto end() const {
    return sentinel<true>{ranges::end(base_), pred_.operator->()};
  }

 private:
  V base_{};
  movable_box<Pred> pred_{};
};

#if __cplusplus >= 201703L

template<typename R, typename Pred>
take_while_view(R&&, Pred) -> take_while_view<views::all_t<R>, Pred>;

#endif

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_TAKE_WHILE_VIEW_HPP_
