//
// Created by cosge on 2024-01-03.
//

#ifndef VCCC_ITERATOR_COUNTED_ITERATOR_HPP
#define VCCC_ITERATOR_COUNTED_ITERATOR_HPP

#include <type_traits>
#include <utility>

#include "vccc/core/constexpr.hpp"
#include "vccc/concepts/assignable_from.hpp"
#include "vccc/concepts/common_with.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/iterator/contiguous_iterator.hpp"
#include "vccc/iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/iterator/bidirectional_iterator.hpp"
#include "vccc/iterator/default_sentinel_t.hpp"
#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/input_iterator.hpp"
#include "vccc/iterator/input_or_output_iterator.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/iterator/iter_move.hpp"
#include "vccc/iterator/iter_rvalue_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/iterator/random_access_iterator.hpp"
#include "vccc/memory/to_address.hpp"
#include "vccc/type_traits/void_t.hpp"
#include "vccc/utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace detail {

template<typename I, bool = indirectly_readable<I>::value /* true */>
struct counted_iterator_value_type {
  using value_type = iter_value_t<I>;
};
template<typename I>
struct counted_iterator_value_type<I, false> {};

template<typename I, typename = void>
struct counted_iterator_iterator_concept {};
template<typename I>
struct counted_iterator_iterator_concept<I, void_t<typename I::iterator_concept>> {
  using iterator_concept = typename I::iterator_concept;
};

template<typename I, typename = void>
struct counted_iterator_iterator_category {};
template<typename I>
struct counted_iterator_iterator_category<I, void_t<typename I::iterator_category>> {
  using iterator_category = typename I::iterator_category;
};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename I>
class counted_iterator
    : public detail::counted_iterator_value_type<I>
    , public detail::counted_iterator_iterator_concept<I>
    , public detail::counted_iterator_iterator_category<I>
{
 public:
  static_assert(input_or_output_iterator<I>::value, "Constraints not satisfied");

  using iterator_type = I;
  using difference_type = iter_difference_t<I>;

  constexpr counted_iterator() = default;

  constexpr counted_iterator(I x, iter_difference_t<I> n)
      : current_(std::move(x)), length_(n) {}

  template<typename I2, std::enable_if_t<convertible_to<const I2&, I>::value, int> = 0>
  constexpr counted_iterator(const counted_iterator<I2>& other)
      : current_(other.current_), length_(other.length_) {}


  template<typename I2, std::enable_if_t<assignable_from<I&, const I2&>::value, int> = 0>
  constexpr counted_iterator& operator=(const counted_iterator<I2>& other) {
    current_ = other.current_;
    length_ = other.length_;
    return *this;
  }


  constexpr const I& base() const& noexcept {
    return current_;
  }
  constexpr I base() && {
    return std::move(current_);
  }


  constexpr difference_type count() const noexcept {
    return length_;
  }


  constexpr decltype(auto) operator*() {
    return *current_;
  }
  template<typename I2 = I, std::enable_if_t<dereferenceable<const I2>::value, int> = 0>
  constexpr decltype(auto) operator*() const {
    return *current_;
  }
  template<typename I2 = I, std::enable_if_t<contiguous_iterator<I2>::value, int> = 0>
  constexpr auto operator->() const noexcept {
    return vccc::to_address(current_);
  }


  template<typename I2 = I, std::enable_if_t<random_access_iterator<I2>::value, int> = 0>
  constexpr decltype(auto) operator[](iter_difference_t<I> n) const {
    return this->base()[n];
  }


  constexpr counted_iterator& operator++() {
    ++current_;
    --length_;
    return *this;

  }
  template<typename I2 = I, std::enable_if_t<forward_iterator<I2>::value == false, int> = 0>
  VCCC_CONSTEXPR_AFTER_CXX20 decltype(auto) operator++(int) {
    --length_;
    try {
      return current_++;
    } catch (...) {
      ++length_;
      throw;
    }
  }
  template<typename I2 = I, std::enable_if_t<forward_iterator<I2>::value, int> = 0>
  constexpr counted_iterator operator++(int) {
    counted_iterator temp{*this};
    ++*this;
    return temp;
  }
  template<typename I2 = I, std::enable_if_t<bidirectional_iterator<I2>::value, int> = 0>
  constexpr counted_iterator& operator--() {
    --current_;
    ++length_;
    return *this;
  }
  template<typename I2 = I, std::enable_if_t<bidirectional_iterator<I2>::value, int> = 0>
  constexpr counted_iterator operator--(int) {
    counted_iterator temp{*this};
    --*this;
    return temp;
  }

  template<typename I2 = I, std::enable_if_t<random_access_iterator<I2>::value, int> = 0>
  constexpr counted_iterator operator+(iter_difference_t<I> n) const {
    return counted_iterator(current_ + n, length_ - n);
  }
  template<typename I2 = I, std::enable_if_t<random_access_iterator<I2>::value, int> = 0>
  constexpr counted_iterator& operator+=(iter_difference_t<I> n) {
    current_ += n;
    length_ -= n;
    return *this;
  }

  template<typename I2 = I, std::enable_if_t<random_access_iterator<I2>::value, int> = 0>
  constexpr counted_iterator operator-(iter_difference_t<I> n) const {
    return counted_iterator(current_ - n, length_ + n);
  }
  template<typename I2 = I, std::enable_if_t<random_access_iterator<I2>::value, int> = 0>
  constexpr counted_iterator& operator-=(iter_difference_t<I> n) {
    current_ -= n;
    length_ += n;
    return *this;
  }

  template<typename I2, std::enable_if_t<common_with<I, I2>::value, int> = 0>
  friend constexpr bool operator==(const counted_iterator& x, const counted_iterator<I2>& y) {
    using namespace vccc::rel_ops;
    return x.count() == y.count();
  }

  template<typename I2, std::enable_if_t<common_with<I, I2>::value, int> = 0>
  friend constexpr bool operator<(const counted_iterator& x, const counted_iterator<I2>& y) {
    using namespace vccc::rel_ops;
    return y.count() < x.count();
  }

  template<typename I2, std::enable_if_t<common_with<I, I2>::value, int> = 0>
  friend constexpr bool operator!=(const counted_iterator& x, const counted_iterator<I2>& y) {
    using namespace vccc::rel_ops;
    return x.count() != y.count();
  }
  template<typename I2, std::enable_if_t<common_with<I, I2>::value, int> = 0>
  friend constexpr bool operator<=(const counted_iterator& x, const counted_iterator<I2>& y) {
    using namespace vccc::rel_ops;
    return y.count() <= x.count();
  }
  template<typename I2, std::enable_if_t<common_with<I, I2>::value, int> = 0>
  friend constexpr bool operator>(const counted_iterator& x, const counted_iterator<I2>& y) {
    using namespace vccc::rel_ops;
    return y.count() > x.count();
  }
  template<typename I2, std::enable_if_t<common_with<I, I2>::value, int> = 0>
  friend constexpr bool operator>=(const counted_iterator& x, const counted_iterator<I2>& y) {
    using namespace vccc::rel_ops;
    return y.count() >= x.count();
  }


  friend constexpr bool operator==(const counted_iterator& x, default_sentinel_t) {
    return x.count() == 0;
  }
  friend constexpr bool operator!=(const counted_iterator& x, default_sentinel_t) {
    return x.count() != 0;
  }


  template<typename I2 = I>
  friend constexpr std::enable_if_t<random_access_iterator<I2>::value, counted_iterator>
  operator+(iter_difference_t<I> n, const counted_iterator& x) {
    return x + n;
  }

  template<typename I2>
  friend constexpr iter_difference_t<I2>
  operator-(const counted_iterator& x, const counted_iterator<I2>& y) {
    return y.count() - x.count();
  }

  friend constexpr iter_difference_t<I>
  operator-(const counted_iterator& x, default_sentinel_t) {
    return -x.count();
  }

  friend constexpr iter_difference_t<I>
  operator-(default_sentinel_t, const counted_iterator& y) {
    return y.count();
  }

  template<typename I2 = I, std::enable_if_t<input_iterator<I2>::value, int> = 0>
  friend constexpr iter_rvalue_reference_t<I2>
  iter_move(const counted_iterator& i) noexcept(noexcept(ranges::iter_move(i.base()))) {
    return ranges::iter_move(i.base());
  }

  // TODO: implement iter_swap

 private:
  iterator_type current_;
  difference_type length_ = 0;
};

namespace detail {

template<typename I, bool = is_primary_iterator_traits<cxx20_iterator_traits<I>>::value /* true */>
struct counted_iterator_iterator_traits : cxx20_iterator_traits_sfinae<counted_iterator<I>> {};

template<typename I>
struct counted_iterator_iterator_traits<I, false> : cxx20_iterator_traits<I> {
  using pointer = std::conditional_t<
      contiguous_iterator<I>::value,
      std::add_pointer_t<iter_reference_t<I>>,
      void
  >;
};

template<typename I>
struct is_primary_iterator_traits<counted_iterator<I>> : is_primary_iterator_traits<I> {};

} // namespace detail


template<typename I>
struct cxx20_iterator_traits<counted_iterator<I>> : detail::counted_iterator_iterator_traits<I> {
  static_assert(input_iterator<I>::value, "Constraints not satisfied");
};



/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_COUNTED_ITERATOR_HPP