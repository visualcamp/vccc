//
// Created by yonggyulee on 1/26/24.
//

#ifndef VCCC_RANGES_VIEWS_VALUES_VIEW_HPP
#define VCCC_RANGES_VIEWS_VALUES_VIEW_HPP

#include "vccc/__ranges/views/elements.hpp"
#include "vccc/__ranges/views/elements_view.hpp"

namespace vccc {
namespace ranges {

/**
@addtogroup ranges
@{
@defgroup ranges_values_view__class__Adaptors ranges::values_view, views::values
@brief takes a \ref ranges::view "view" consisting of \ref tuple_like "\a tuple-like"" values and produces a view of the
second elements of each tuple

Takes a \ref ranges::view "view" of \a tuple-like values (e.g. [std::tuple](https://en.cppreference.com/w/cpp/utility/tuple)
or [std::pair]((https://en.cppreference.com/w/cpp/utility/tuple))), and produces a view with a \a value-type of the
\a second element of the adapted view's value-type.

@sa [std::views::values_view](https://en.cppreference.com/w/cpp/ranges/values_view)
@}
*/

/// @addtogroup ranges_values_view__class__Adaptors
/// @{

/// @brief An alias for `ranges::elements_view<R, 0>`
template<typename R>
using values_view = elements_view<R, 1>;

namespace views {

VCCC_INLINE_OR_STATIC constexpr detail::elements_adaptor_object<1> values{};

} // namespace views

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_VALUES_VIEW_HPP
