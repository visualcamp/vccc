//
// Created by yonggyulee on 2024/01/02.
//

#ifndef VCCC_RANGES_VIEWS_EMPTY_VIEW_HPP
#define VCCC_RANGES_VIEWS_EMPTY_VIEW_HPP

#include <cstddef>
#include <type_traits>

#include "vccc/core/inline_or_static.hpp"
#include "vccc/ranges/enable_borrowed_range.hpp"
#include "vccc/ranges/view_interface.hpp"

namespace vccc {
namespace ranges {

/**
@addtogroup ranges
@{
@defgroup ranges_empty_view__class__Factories ranges::empty_view, views::empty
@brief an empty \ref ranges::view "view" with no elements

@sa [std::ranges::empty_view](https://en.cppreference.com/w/cpp/ranges/empty_view)
@}
*/

template<typename T>
class empty_view : public ranges::view_interface<empty_view<T>> {
 public:
  static_assert(std::is_object<T>::value, "Constraints not satisfied");

  static constexpr T* begin() noexcept { return nullptr; }

  static constexpr T* end() noexcept { return nullptr; }

  static constexpr T* data() noexcept { return nullptr; }

  static constexpr std::size_t size() noexcept { return 0; }

  static constexpr bool empty() noexcept { return true; }
};

template<typename T>
struct enable_borrowed_range<empty_view<T>> : std::true_type {};

namespace views {

template<typename T>
VCCC_INLINE_OR_STATIC constexpr empty_view<T> empty{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_EMPTY_VIEW_HPP
