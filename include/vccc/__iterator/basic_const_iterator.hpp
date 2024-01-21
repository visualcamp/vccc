//
// Created by yonggyulee on 1/15/24.
//

#ifndef VCCC_ITERATOR_BASIC_CONST_ITERATOR_HPP
#define VCCC_ITERATOR_BASIC_CONST_ITERATOR_HPP

#include <memory>
#include <type_traits>
#include <utility>

#include "vccc/__concepts/common_with.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/different_from.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__concepts/semiregular.hpp"
#include "vccc/__concepts/totally_ordered.hpp"
#include "vccc/__iterator/bidirectional_iterator.hpp"
#include "vccc/__iterator/contiguous_iterator.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/input_iterator.hpp"
#include "vccc/__iterator/iter_const_reference_t.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iter_common_reference_t.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__iterator/random_access_iterator.hpp"
#include "vccc/__memory/to_address.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_specialization.hpp"
#include "vccc/__type_traits/is_nothrow_convertible.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"
#include "vccc/__type_traits/type_identity.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace detail {

template<typename Iter, bool = forward_iterator<Iter>::value /* true */>
struct basic_const_iterator_category {
  using iterator_category = typename cxx20_iterator_traits<Iter>::iterator_category;
};

template<typename Iter>
struct basic_const_iterator_category<Iter, false> {};

template<typename Iter>
struct basic_const_iterator_concept {
  using iterator_concept =
    std::conditional_t<
      contiguous_iterator<Iter>::value, contiguous_iterator_tag,
    std::conditional_t<
      random_access_iterator<Iter>::value, random_access_iterator_tag,
    std::conditional_t<
      bidirectional_iterator<Iter>::value, bidirectional_iterator_tag,
    std::conditional_t<
      forward_iterator<Iter>::value, forward_iterator_tag,
      input_iterator_tag
    >>>>;
};

template<typename I, bool = input_iterator<I>::value /* false */>
struct constant_iterator : std::false_type {};
template<typename I>
struct constant_iterator<I, true>
    : same_as<iter_const_reference_t<I>, iter_reference_t<I>> {};

}

template<typename Iter>
class basic_const_iterator : public detail::basic_const_iterator_category<Iter> {
  using reference = iter_const_reference_t<Iter>;
  using rvalue_reference = common_reference_t<const iter_value_t<Iter>&&, iter_rvalue_reference_t<Iter>>;
 public:
  static_assert(input_iterator<Iter>::value, "Constraints not satisfied");

  using iterator_concept = typename detail::basic_const_iterator_concept<Iter>::iterator_concept;
  using value_type = iter_value_t<Iter>;
  using difference_type = iter_difference_t<Iter>;

  basic_const_iterator() = default;

  constexpr basic_const_iterator(Iter x) : current_(std::move(x)) {}

  template<typename U, std::enable_if_t<convertible_to<U, Iter>::value, int> = 0>
  constexpr basic_const_iterator(basic_const_iterator<U> other)
      : current_(std::move(other.current_)) {}

  template<typename T, std::enable_if_t<conjunction<
      different_from<T, basic_const_iterator>,
      convertible_to<T, Iter>
  >::value, int> = 0>
  constexpr basic_const_iterator(T&& x)
      : current_(std::forward<T>(x)) {}

  // TODO: Implement operator= or not

  constexpr const Iter& base() const& noexcept {
    return current_;
  }

  constexpr Iter base() && {
    return std::move(current_);
  }

  constexpr reference operator*() const {
    return static_cast<reference>(*base());
  }

  template<typename I = Iter, std::enable_if_t<conjunction<
      std::is_lvalue_reference<iter_reference_t<I>>,
      same_as<remove_cvref_t<iter_reference_t<I>>, value_type>
  >::value, int> = 0>
  constexpr const auto* operator->() const {
    return arrow(contiguous_iterator<Iter>{});
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr iter_common_reference_t<I> operator[](difference_type n) const {
    return static_cast<iter_common_reference_t<Iter>>(base()[n]);
  }

  constexpr basic_const_iterator& operator++() {
    ++current_;
    return *this;
  }

  template<typename I = Iter, std::enable_if_t<forward_iterator<I>::value == false, int> = 0>
  constexpr void operator++(int) {
    ++current_;
  }

  template<typename I = Iter, std::enable_if_t<forward_iterator<I>::value, int> = 0>
  constexpr basic_const_iterator operator++(int) {
    auto tmp = *this;
    ++*this;
    return tmp;
  }

  template<typename I = Iter, std::enable_if_t<bidirectional_iterator<I>::value, int> = 0>
  constexpr basic_const_iterator& operator--() {
    --current_;
    return *this;
  }

  template<typename I = Iter, std::enable_if_t<bidirectional_iterator<I>::value, int> = 0>
  constexpr basic_const_iterator operator--(int) {
    auto tmp = *this;
    --*this;
    return tmp;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr basic_const_iterator& operator+=(difference_type n) {
    current_ += n;
    return *this;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr basic_const_iterator& operator-=(difference_type n) {
    current_ -= n;
    return *this;
  }

  template<typename Other, std::enable_if_t<conjunction<
      negation< is_specialization<Other, basic_const_iterator> >,
      detail::constant_iterator<Other>,
      convertible_to<const Iter&, Other>
  >::value, int> = 0>
  constexpr operator Other() const& noexcept(is_nothrow_convertible<const Iter&, Other>::value) {
    return current_;
  }

  template<typename Other, std::enable_if_t<conjunction<
      negation< is_specialization<Other, basic_const_iterator> >,
      detail::constant_iterator<Other>,
      convertible_to<Iter, Other>
  >::value, int> = 0>
  constexpr operator Other() && noexcept(is_nothrow_convertible<Iter, Other>::value) {
    return std::move(current_);
  }

  template<typename S, std::enable_if_t<sentinel_for<S, Iter>::value, int> = 0>
  constexpr bool operator==(const S& s) const {
    using namespace vccc::rel_ops;
    return base() == s;
  }

  template<typename S, std::enable_if_t<conjunction<
      different_from<S, basic_const_iterator>,
      sentinel_for<S, Iter>
  >::value, int> = 0>
  friend constexpr bool operator==(const S& s, const basic_const_iterator& i) {
    using namespace vccc::rel_ops;
    return i == s;
  }

  template<typename S, std::enable_if_t<sentinel_for<S, Iter>::value, int> = 0>
  constexpr bool operator!=(const S& s) const {
    return !(*this == s);
  }

  template<typename S, std::enable_if_t<conjunction<
      different_from<S, basic_const_iterator>,
      sentinel_for<S, Iter>
  >::value, int> = 0>
  friend constexpr bool operator!=(const S& s, const basic_const_iterator& i) {
    return i != s;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr bool operator<(basic_const_iterator& y) const {
    return base() < y;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr bool operator>(basic_const_iterator& y) const {
    using namespace vccc::rel_ops;
    return base() > y;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr bool operator<=(basic_const_iterator& y) const {
    using namespace vccc::rel_ops;
    return base() <= y;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  constexpr bool operator>=(basic_const_iterator& y) const {
    using namespace vccc::rel_ops;
    return base() >= y;
  }

  template<typename I, typename I2 = Iter, std::enable_if_t<conjunction<
      different_from<I, basic_const_iterator>,
      random_access_iterator<I2>,
      totally_ordered_with<I2, I>
  >::value, int> = 0>
  constexpr bool operator<(I& y) const {
    using namespace vccc::rel_ops;
    return base() < y;
  }

  template<typename I, typename I2 = Iter, std::enable_if_t<conjunction<
      different_from<I, basic_const_iterator>,
      random_access_iterator<I2>,
      totally_ordered_with<I2, I>
  >::value, int> = 0>
  constexpr bool operator>(I& y) const {
    using namespace vccc::rel_ops;
    return base() > y;
  }

  template<typename I, typename I2 = Iter, std::enable_if_t<conjunction<
      different_from<I, basic_const_iterator>,
      random_access_iterator<I2>,
      totally_ordered_with<I2, I>
  >::value, int> = 0>
  constexpr bool operator<=(I& y) const {
    using namespace vccc::rel_ops;
    return base() <= y;
  }

  template<typename I, typename I2 = Iter, std::enable_if_t<conjunction<
      different_from<I, basic_const_iterator>,
      random_access_iterator<I2>,
      totally_ordered_with<I2, I>
  >::value, int> = 0>
  constexpr bool operator>=(I& y) const {
    using namespace vccc::rel_ops;
    return base() >= y;
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  friend constexpr basic_const_iterator operator+(const basic_const_iterator& i, difference_type n) {
    return basic_const_iterator<I>(i.base() + n);
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  friend constexpr basic_const_iterator operator+(difference_type n, const basic_const_iterator& i) {
    return basic_const_iterator<I>(i.base() + n);
  }

  template<typename I = Iter, std::enable_if_t<random_access_iterator<I>::value, int> = 0>
  friend constexpr basic_const_iterator operator-(const basic_const_iterator& i, difference_type n) {
    return basic_const_iterator<I>(i.base() - n);
  }

  template<typename S, std::enable_if_t<sized_sentinel_for<S, Iter>::value, int> = 0>
  constexpr difference_type operator-(const S& s) const {
    return current_ - s;
  }

  template<typename S, std::enable_if_t<conjunction<
      negation< is_specialization<S, basic_const_iterator> >,
      sized_sentinel_for<S, Iter>
  >::value, int> = 0>
  friend constexpr difference_type operator-(const S& s, const basic_const_iterator& i) {
    return s - i.base();
  }

  friend constexpr rvalue_reference
  iter_move(const basic_const_iterator& i)
      noexcept(noexcept(static_cast<rvalue_reference>(ranges::iter_move(i.base()))))
  {
    return static_cast<rvalue_reference>(vccc::ranges::iter_move(i));
  }

 private:
  constexpr const auto* arrow(std::true_type /* contiguous_iterator */) const {
    return vccc::to_address(base());
  }
  constexpr const auto* arrow(std::false_type /* contiguous_iterator */) const {
    return std::addressof(*base());
  }

  Iter current_;
};

template<typename I>
using const_iterator = std::enable_if_t< input_iterator<I>::value,
    std::conditional_t<
        detail::constant_iterator<I>::value,
        I,
        basic_const_iterator<I>
    >
>;

template<typename S>
using const_sentinel = std::enable_if_t< semiregular<S>::value,
    std::conditional_t<
        input_iterator<S>::value,
        const_iterator<S>,
        S
    >
>;

template<typename I, std::enable_if_t<input_iterator<I>::value, int> = 0>
constexpr const_iterator<I> make_const_iterator(I it) { return it; }

template<typename S, std::enable_if_t<semiregular<S>::value, int> = 0>
constexpr const_sentinel<S> make_const_sentinel(S s) { return s; }

namespace detail {

template<typename T, typename U, bool = conjunction<common_with<U, T>>::value>
struct basic_const_iterator_common_type
    : std::conditional_t<
          input_iterator<common_type_t<T, U>>::value,
          type_identity< basic_const_iterator<common_type_t<T, U>> >,
          no_common_type
      > {};
template<typename T, typename U>
struct basic_const_iterator_common_type<T, U, false> {};

} // namespace detail

template<typename T, typename U>
struct common_type<basic_const_iterator<T>, U>
    : detail::basic_const_iterator_common_type<T, U> {};

template<typename T, typename U>
struct common_type<T, basic_const_iterator<U>>
    : detail::basic_const_iterator_common_type<T, U> {};

template<typename T, typename U>
struct common_type<basic_const_iterator<T>, basic_const_iterator<U>>
    : detail::basic_const_iterator_common_type<T, U> {};




} // namespace vccc

#endif // VCCC_ITERATOR_BASIC_CONST_ITERATOR_HPP