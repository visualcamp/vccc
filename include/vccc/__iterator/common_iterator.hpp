//
// Created by cosge on 2024-01-26.
//

#ifndef VCCC_ITERATOR_COMMON_ITERATOR_HPP_
#define VCCC_ITERATOR_COMMON_ITERATOR_HPP_

#include <new>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/copyable.hpp"
#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__concepts/different_from.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__concepts/move_constructible.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/incrementable_traits.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/input_or_output_iterator.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/detail/tag.hpp"
#include "vccc/__type_traits/has_operator_arrow.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"
#include "vccc/__utility/in_place.hpp"
#include "vccc/variant.hpp"

namespace vccc {

template<typename I, typename S>
class common_iterator {
  class proxy {
    friend class common_iterator;

    iter_value_t<I> keep_;
    constexpr proxy(iter_reference_t<I>&& x)
        : keep_(std::forward<iter_reference_t<I>>(x)) {}

   public:
    constexpr const iter_value_t<I>* operator->() const noexcept {
      return vccc::addressof(keep_);
    }

  };

  class postfix_proxy {
    friend class common_iterator;

    iter_value_t<I> keep_;
    constexpr postfix_proxy(iter_reference_t<I>&& x)
        : keep_(std::forward<iter_reference_t<I>>(x)) {}

   public:
    constexpr const iter_value_t<I>& operator*() const noexcept {
      return keep_;
    }
  };

 public:
  static_assert(input_or_output_iterator<I>::value, "Constraints not satisfied");
  static_assert(sentinel_for<S, I>::value, "Constraints not satisfied");
  static_assert(negation<same_as<I, S>>::value, "Constraints not satisfied");
  static_assert(copyable<I>::value, "Constraints not satisfied");

  common_iterator() = default;

  constexpr common_iterator(I i) : var_(std::move(i)) {}
  constexpr common_iterator(S s) : var_(std::move(s)) {}

  template<typename I2, typename S2, std::enable_if_t<conjunction<
    convertible_to<const I2&, I>,
    convertible_to<const S2&, S>
  >::value, int> = 0>
  constexpr common_iterator(const common_iterator<I2, S2>& x)
      noexcept(conjunction<
          std::is_nothrow_constructible<I, const I2&>,
          std::is_nothrow_constructible<S, const S2&>>::value)
      : var_{detail::variant_valueless_ctor{}}
  {
    switch (x.var_.index()) {
      case 0:
        var_.emplace<0>(detail::variant_raw_get(x.var_._base().storage(), in_place_index<0>));
        break;
      case 1:
        var_.emplace<1>(detail::variant_raw_get(x.var_._base().storage(), in_place_index<1>));
        break;
      default:
        break;
    }
  }

  template<typename I2, typename S2>
  constexpr common_iterator& operator=(const common_iterator<I2, S2>& x)
      noexcept(conjunction<
          std::is_nothrow_constructible<I, const I2&>, std::is_nothrow_assignable<I, const I2&>,
          std::is_nothrow_constructible<S, const S2&>, std::is_nothrow_assignable<S, const S2&> >::value)
  {
    if (var_.index() == x.var_.index()) {
      switch (x.var_.index()) {
        case 0:
          this->iterator() = detail::variant_raw_get(x.var_._base().storage(), in_place_index<0>);
          return *this;
        case 1:
          this->sentinel() = detail::variant_raw_get(x.var_._base().storage(), in_place_index<1>);
          return *this;
        default:
          break;
      }
    }

    switch (x.var_.index()) {
      case 0:
        var_.emplace<0>(detail::variant_raw_get(x._base().storage(), in_place_index<0>));
        break;
      case 1:
        var_.emplace<1>(detail::variant_raw_get(x._base().storage(), in_place_index<1>));
        break;
      default:
        break;
    }

    return *this;
  }

  constexpr decltype(auto) operator*() {
    return *this->iterator();
  }

  template<typename I2 = I, std::enable_if_t<dereferenceable<const I2>::value, int> = 0>
  constexpr decltype(auto) operator*() const {
    return *this->iterator();
  }

  template<typename I2 = I, std::enable_if_t<conjunction<
      indirectly_readable<const I2>,
      disjunction<
        std::is_pointer<I2>,
        has_operator_arrow<const I2&>,
        std::is_reference<iter_reference_t<I2>>,
        constructible_from<iter_value_t<I2>, iter_reference_t<I2>>
      >
  >::value, int> = 0>
  constexpr auto operator->() const {
    using tag =
        detail::conditional_tag<
            disjunction<std::is_pointer<I>, has_operator_arrow<const I&>>,
            std::is_reference<iter_reference_t<I>>
        >;
    return operator_arrow(tag{});
  }

  constexpr common_iterator& operator++() {
    ++this->iterator();
    return *this;
  }

  constexpr decltype(auto) operator++(int) {
    using tag =
        detail::conditional_tag<
            forward_iterator<I>,
            disjunction<
                conjunction<
                    detail::is_post_incrementable<I>,
                    dereferenceable<I>
                >,
                negation< indirectly_readable<I> >,
                negation< constructible_from<iter_value_t<I>, iter_reference_t<I>> >,
                negation< move_constructible<iter_value_t<I>> >
            >
        >;
    return post_increment(tag{});
  }

  template<typename I2, typename S2, std::enable_if_t<conjunction<
      sentinel_for<S2, I>,
      sentinel_for<S, I2>,
      negation< equality_comparable_with<I, I2> >
  >::value, int> = 0>
  friend constexpr bool operator==(const common_iterator& x, const common_iterator<I2, S2>& y) {
    if (x.var_.index() == y.var_.index())
      return true;

    using namespace vccc::rel_ops;
    switch (x.var_.index()) {
      case 0:
        switch(y.var_.index()) {
          case 0: return true;
          case 1: return x.iterator() == y.sentinel();
          default: return false;
        }
      case 1:
        switch(y.var_.index()) {
          case 0: return x.sentinel() == y.iterator();
          case 1: return true;
          default: return false;
        }
      default:
        return false;
    }
  }

  template<typename I2, typename S2, std::enable_if_t<conjunction<
      sentinel_for<S2, I>,
      sentinel_for<S, I2>,
      negation< equality_comparable_with<I, I2> >
  >::value, int> = 0>
  friend constexpr bool operator!=(const common_iterator& x, const common_iterator<I2, S2>& y) {
    return !(x == y);
  }

  template<typename I2, typename S2, std::enable_if_t<conjunction<
      sentinel_for<S2, I>,
      sentinel_for<S, I2>,
      equality_comparable_with<I, I2>
  >::value, int> = 0>
  friend constexpr bool operator==(const common_iterator& x, const common_iterator<I2, S2>& y) {
    using namespace vccc::rel_ops;
    switch (x.var_.index()) {
      case 0:
        switch(y.var_.index()) {
          case 0: return x.iterator() == y.iterator();
          case 1: return x.iterator() == y.sentinel();
          default: return false;
        }
      case 1:
        switch(y.var_.index()) {
          case 0: return x.sentinel() == y.iterator();
          case 1: return true;
          default: return false;
        }
      default:
        return false;
    }
  }

  template<typename I2, typename S2, std::enable_if_t<conjunction<
      sentinel_for<S2, I>,
      sentinel_for<S, I2>,
      equality_comparable_with<I, I2>
  >::value, int> = 0>
  friend constexpr bool operator!=(const common_iterator& x, const common_iterator<I2, S2>& y) {
    return !(x == y);
  }


  template<typename I2, typename S2, std::enable_if_t<conjunction<
      sized_sentinel_for<I2, I>,
      sized_sentinel_for<S2, I>,
      sentinel_for<S, I2>
  >::value, int> = 0>
  friend constexpr iter_difference_t<I2> operator-(const common_iterator& x, const common_iterator<I2, S2>& y) {
    switch (x.var_.index()) {
      case 0:
        switch(y.var_.index()) {
          case 0: return x.iterator() - y.iterator();
          case 1: return x.iterator() - y.sentinel();
          default: return 0;
        }
      case 1:
        switch(y.var_.index()) {
          case 0: return x.sentinel() - y.iterator();
          case 1: return 0;
          default: return 0;
        }
      default:
        return 0;
    }
  }

 private:
  decltype(auto) iterator() {
    return detail::variant_raw_get(var_._base().storage(), in_place_index<0>);
  }
  decltype(auto) iterator() const {
    return detail::variant_raw_get(var_._base().storage(), in_place_index<0>);
  }
  decltype(auto) sentinel() {
    return detail::variant_raw_get(var_._base().storage(), in_place_index<1>);
  }
  decltype(auto) sentinel() const {
    return detail::variant_raw_get(var_._base().storage(), in_place_index<1>);
  }

  constexpr auto operator_arrow(detail::tag_1) const {
    return this->iterator();
  }
  constexpr auto operator_arrow(detail::tag_2) const {
    auto&& tmp = **this;
    return vccc::addressof(tmp);
  }
  constexpr auto operator_arrow(detail::tag_else) const {
    return proxy(**this);
  }

  constexpr decltype(auto) post_increment(detail::tag_1) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }
  constexpr decltype(auto) post_increment(detail::tag_2) {
    return this->iterator()++;
  }
  constexpr postfix_proxy post_increment(detail::tag_else) {
    postfix_proxy p(**this);
    ++*this;
    return p;
  }

  variant<I, S> var_{};
};

template<typename I, typename S>
struct incrementable_traits<common_iterator<I, S>> {
  using difference_type = iter_difference_t<I>;
};

namespace detail {

template<typename I, bool = std::is_integral<iter_difference_t<I>>::value /* false */>
struct common_iterator_category {
#if __cplusplus < 202002L
  using iterator_category = iterator_ignore;
#endif
};

template<typename I, bool v = has_typename_iterator_category<cxx20_iterator_traits<I>>::value /* false */>
struct common_iterator_category_check_forward : std::false_type {};
template<typename I>
struct common_iterator_category_check_forward<I, true>
    : derived_from<typename cxx20_iterator_traits<I>::iterator_category, forward_iterator_tag> {};

template<typename I>
struct common_iterator_category<I, true> {
  using iterator_category =
      std::conditional_t<
          common_iterator_category_check_forward<I>::value,
          forward_iterator_tag,
          input_iterator_tag
      >;
};

template<typename I, typename S, bool = has_operator_arrow<const common_iterator<I, S>&>::value /* false */>
struct common_iterator_pointer {
  using type = void;
};
template<typename I, typename S>
struct common_iterator_pointer<I, S, true> {
  using type = decltype(std::declval<const common_iterator<I, S>&>().operator->());
};

} // namespace detail

template<typename I, typename S>
struct cxx20_iterator_traits<common_iterator<I, S>> : detail::common_iterator_category<I> {
  using iterator_concept = std::conditional_t<forward_iterator<I>::value, forward_iterator_tag, input_iterator_tag>;
  using value_type = iter_value_t<I>;
  using difference_type = iter_difference_t<I>;
  using pointer = typename detail::common_iterator_pointer<I, S>::type;
  using reference = iter_reference_t<I>;
};

} // namespace vccc

#endif // VCCC_ITERATOR_COMMON_ITERATOR_HPP_
