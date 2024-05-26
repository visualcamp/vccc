//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_RANGES_VIEWS_SINGLE_HPP
#define VCCC_RANGES_VIEWS_SINGLE_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__ranges/movable_box.hpp"
#include "vccc/__ranges/view_interface.hpp"
#include "vccc/__utility/in_place.hpp"

namespace vccc {
namespace ranges {

/**
@addtogroup ranges
@{
@defgroup ranges_single_view__class__Factories ranges::single_view, views::single
@brief a \ref ranges::view "view" that contains a single element of a specified value

Produces a \ref ranges::view "view" that contains exactly one element of a specified value.

The lifetime of the element is bound to the parent `%single_view`. Copying `%single_view` makes a copy of the element.
@sa [std::ranges::single_view](https://en.cppreference.com/w/cpp/ranges/single_view)
@}
*/

/// @addtogroup ranges_single_view__class__Factories
/// @{

template<typename T>
class single_view : public ranges::view_interface<single_view<T>> {
 public:
  static_assert(copy_constructible<T>::value, "Constraints not satisfied");
  static_assert(std::is_object<T>::value, "Constraints not satisfied");

  constexpr single_view() = default;

  constexpr explicit single_view(const T& t)
      : value_(t) {}

  constexpr explicit single_view(T&& t)
      : value_(std::move(t)) {}

  template<typename InPlace, typename... Args, std::enable_if_t<conjunction<
      std::is_same<InPlace, vccc::in_place_t>,
      constructible_from<T, Args...>
  >::value, int> = 0>
  constexpr explicit single_view(InPlace, Args&&... args)
      : value_{vccc::in_place, std::forward<Args>(args)...} {}

  constexpr T* begin() noexcept { return data(); }
  constexpr const T* begin() const noexcept { return data(); }

  constexpr T* end() noexcept { return data() + 1; }
  constexpr const T* end() const noexcept { return data() + 1; }

  static constexpr std::size_t size() noexcept { return 1; }

  constexpr T* data() noexcept { return value_.operator->(); }
  constexpr const T* data() const noexcept { return value_.operator->(); }

 private:
  movable_box<T> value_;
};

#if __cplusplus >= 201703L

template<typename T>
single_view(T) -> single_view<T>;

#endif

/// @}

namespace views {
namespace detail {

struct single_niebloid {
  template<typename T, std::enable_if_t<conjunction<
      copy_constructible<std::decay_t<T>>,
      std::is_object<std::decay_t<T>>,
      std::is_constructible<std::decay_t<T>, T&&>
  >::value, int> = 0>
  constexpr single_view<std::decay_t<T>>
  operator()(T&& t) const {
    return single_view<std::decay_t<T>>(std::forward<T>(t));
  }
};

} // namespace detail

namespace niebloid {

/// @addtogroup ranges_single_view__class__Factories
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::single_niebloid single{};

/// @}

} // namespace niebloid
using namespace niebloid;

} // namespace vccc
} // namespace ranges
} // namespace views

#endif // VCCC_RANGES_VIEWS_SINGLE_HPP
