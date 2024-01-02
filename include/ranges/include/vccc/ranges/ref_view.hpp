//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_RANGES_REF_VIEW_HPP
#define VCCC_RANGES_REF_VIEW_HPP

#include <memory>
#include <type_traits>

#include "vccc/ranges/contiguous_range.hpp"
#include "vccc/ranges/enable_borrowed_range.hpp"
#include "vccc/ranges/begin.hpp"
#include "vccc/ranges/data.hpp"
#include "vccc/ranges/empty.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/sized_range.hpp"
#include "vccc/ranges/view_interface.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_invocable.hpp"
#include "vccc/type_traits/negation.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {

/// @addtogroup
/// @{

/**
@brief 	a \ref ranges::view "view" of the elements of some other \ref ranges::range "range"

`%ref_view` is a \ref ranges::view "view" of the elements of some other \ref ranges::range "range". It wraps a reference
to that range.
 */
template<typename R>
class ref_view : public view_interface<ref_view<R>> {
 public:
  static_assert(std::is_object<R>::value, "Constraints not satisfied");

  template<typename T, std::enable_if_t<conjunction<
      negation< std::is_same<ref_view, remove_cvref_t<T>> >,
      std::is_lvalue_reference<T&&>
  >::value, int> = 0>
  constexpr ref_view(T&& t) noexcept
      : r_(std::addressof(static_cast<R&>(std::forward<T>(t)))) {}

  constexpr R& base() const {
    return *r_;
  }

  constexpr ranges::iterator_t<R> begin() const {
    return ranges::begin(*r_);
  }

  constexpr ranges::sentinel_t<R> end() const {
    return ranges::end(*r_);
  }

  template<typename T = R, std::enable_if_t<
      is_invocable<decltype(ranges::empty), R&>::value, int> = 0>
  constexpr bool empty() const {
    return ranges::empty(*r_);
  }

  template<typename T = R, std::enable_if_t<
      ranges::sized_range<R>::value, int> = 0>
  constexpr bool size() const {
    return ranges::size(*r_);
  }

  template<typename T = R, std::enable_if_t<
      ranges::contiguous_range<R>::value, int> = 0>
  constexpr auto data() const {
    return ranges::data(*r_);
  }

 private:
  R* r_;
};

#if __cplusplus >= 201703L

template<typename R>
rev_view(R&) -> ref_view<R>;

#endif

template<typename T>
struct enable_borrowed_range<ranges::ref_view<T>> : std::true_type {};

/// @}

} // namespace ranges
} // namespace namespace vccc

#endif // VCCC_RANGES_REF_VIEW_HPP
