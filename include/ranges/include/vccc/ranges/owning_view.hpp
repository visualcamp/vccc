//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_RANGES_OWINING_VIEW_HPP
#define VCCC_RANGES_OWINING_VIEW_HPP

#include <utility>

#include "vccc/concepts/movable.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/contiguous_range.hpp"
#include "vccc/ranges/data.hpp"
#include "vccc/ranges/empty.hpp"
#include "vccc/ranges/enable_borrowed_range.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/size.hpp"
#include "vccc/ranges/sized_range.hpp"
#include "vccc/ranges/view_interface.hpp"
#include "vccc/type_traits/is_initializer_list.hpp"
#include "vccc/type_traits/is_invocable.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup
/// @{

/**
@brief a \ref ranges::view "view" with unique ownership of some \ref ranges::range "range"

`%owning_view` is a \ref ranges::view "view" that has unique ownership of a  \ref ranges::range "range". It is move-only
and stores that range within it.
 */
template<typename R>
class owning_view : public view_interface<owning_view<R>> {
 public:
  static_assert(ranges::range<R>::value, "Constraints not satisfied");
  static_assert(movable<R>::value, "Constraints not satisfied");
  static_assert(is_initializer_list<R>::value == false, "Constraints not satisfied");

  constexpr owning_view() = default;
  constexpr owning_view(owning_view&&) = default;
  constexpr owning_view(const owning_view&) = delete;
  constexpr owning_view(R&& t) : r_(std::move(t)) {}


  owning_view& operator=(owning_view&&) = default;
  owning_view& operator=(const owning_view&) = delete;


  constexpr R& base() & noexcept { return r_; }
  constexpr const R& base() const & noexcept { return r_; }
  constexpr R&& base() && noexcept { return std::move(r_); }
  constexpr const R&& base() const && noexcept { return std::move(r_); }


  template<typename T = R>
  constexpr ranges::iterator_t<T> begin() {
    return ranges::begin(r_);
  }
  template<typename T = R, std::enable_if_t<ranges::range<const T>::value, int> = 0>
  constexpr auto begin() const {
    return ranges::begin(r_);
  }


  template<typename T = R>
  constexpr ranges::sentinel_t<T> end() {
    return ranges::end(r_);
  }
  template<typename T = R, std::enable_if_t<ranges::range<const T>::value, int> = 0>
  constexpr auto end() const {
    return ranges::end(r_);
  }


  template<typename T = R, std::enable_if_t<is_invocable<decltype(ranges::end), T&>::value, int> = 0>
  constexpr bool empty() {
    return ranges::empty(r_);
  }

  template<typename T = R, std::enable_if_t<is_invocable<decltype(ranges::end), const T&>::value, int> = 0>
  constexpr bool empty() const {
    return ranges::empty(r_);
  }


  template<typename T = R, std::enable_if_t<sized_range<T>::value, int> = 0>
  constexpr auto size() {
    return ranges::size(r_);
  }

  template<typename T = R, std::enable_if_t<sized_range<const T>::value, int> = 0>
  constexpr auto size() const {
    return ranges::size(r_);
  }


  template<typename T = R, std::enable_if_t<contiguous_range<T>::value, int> = 0>
  constexpr auto data() {
    return ranges::data(r_);
  }

  template<typename T = R, std::enable_if_t<contiguous_range<const T>::value, int> = 0>
  constexpr auto data() const {
    return ranges::data(r_);
  }

 private:
  R r_;
};

#if __cplusplus >= 201703L

template<typename R>
owning_view(R&&) -> owning_view<R>;

#endif

template<typename T>
struct enable_borrowed_range<owning_view<T>> : enable_borrowed_range<T> {};

/// @}

} // namespace ranges
} // namespace namespace vccc

#endif // VCCC_RANGES_OWINING_VIEW_HPP
