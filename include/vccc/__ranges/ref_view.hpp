//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_RANGES_REF_VIEW_HPP
#define VCCC_RANGES_REF_VIEW_HPP

#include <memory>
#include <type_traits>

#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/different_from.hpp"
#include "vccc/__memory/addressof.hpp"
#include "vccc/__ranges/contiguous_range.hpp"
#include "vccc/__ranges/enable_borrowed_range.hpp"
#include "vccc/__ranges/begin.hpp"
#include "vccc/__ranges/data.hpp"
#include "vccc/__ranges/empty.hpp"
#include "vccc/__ranges/end.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/size.hpp"
#include "vccc/__ranges/sized_range.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"

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
      different_from<T, ref_view>,
      convertible_to<T, R&>,
      std::is_lvalue_reference<T&&>
  >::value, int> = 0>
  VCCC_ADDRESSOF_CONSTEXPR ref_view(T&& t) noexcept
      : r_(vccc::addressof(static_cast<R&>(std::forward<T>(t)))) {}

  constexpr R& base() const {
    return *r_;
  }

  constexpr iterator_t<R> begin() const {
    return ranges::begin(*r_);
  }

  constexpr sentinel_t<R> end() const {
    return ranges::end(*r_);
  }

  template<typename T = R, std::enable_if_t<
      is_invocable<decltype(ranges::empty), T&>::value, int> = 0>
  constexpr bool empty() const {
    return ranges::empty(*r_);
  }

  template<typename T = R, std::enable_if_t<
      sized_range<T>::value, int> = 0>
  constexpr auto size() const {
    return ranges::size(*r_);
  }

  template<typename T = R, std::enable_if_t<
      contiguous_range<T>::value, int> = 0>
  constexpr auto data() const {
    return ranges::data(*r_);
  }

 private:
  R* r_;
};

#if __cplusplus >= 201703L

template<typename R>
ref_view(R&) -> ref_view<R>;

#endif

template<typename T>
struct enable_borrowed_range<ref_view<T>> : std::true_type {};

/// @}

} // namespace ranges
} // namespace namespace vccc

#endif // VCCC_RANGES_REF_VIEW_HPP
