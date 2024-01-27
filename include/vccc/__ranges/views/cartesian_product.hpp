//
// Created by yonggyulee on 2024/01/27.
//

#ifndef VCCC_RANGES_VIEWS_CARTESIAN_PRODUCT_HPP
#define VCCC_RANGES_VIEWS_CARTESIAN_PRODUCT_HPP

#include <tuple>
#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/cartesian_product_view.hpp"
#include "vccc/__ranges/views/single.hpp"

#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct cartesian_product_niebloid {
  constexpr auto operator()() const {
    return views::single(std::tuple<>());
  }

  template<typename... Rs, std::enable_if_t<conjunction<
      viewable_range<Rs>...
  >::value, int> = 0>
  constexpr auto operator()(Rs&&... rs) const {
    return ranges::cartesian_product_view<all_t<Rs&&>...>(std::forward<Rs>(rs)...);
  }
};

} // namespace detail
/**
@addtogroup ranges
@{
@defgroup cartesian_product_view__class__Factories ranges::cartesian_product_view, views::cartesian_product
@brief a \ref ranges::view "view" consisting of tuples of results calculated by the n-ary cartesian product of the adapted views

@sa [std::ranges::cartesian_product_view, std::views::cartesian_product](https://en.cppreference.com/w/cpp/ranges/cartesian_product_view)
@}
*/

/// @addtogroup cartesian_product_view__class__Factories
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::cartesian_product_niebloid cartesian_product{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_CARTESIAN_PRODUCT_HPP
