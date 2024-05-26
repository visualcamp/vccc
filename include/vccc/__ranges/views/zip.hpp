//
// Created by YongGyu Lee on 4/9/24.
//

#ifndef VCCC_RANGES_VIEWS_ZIP_HPP_
#define VCCC_RANGES_VIEWS_ZIP_HPP_

#include <tuple>
#include <type_traits>

#include "vccc/__core/decay_copy.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/empty_view.hpp"
#include "vccc/__ranges/views/zip_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct zip_niebloid {
  constexpr auto operator()() const {
    return vccc_decay_copy(views::empty<std::tuple<>>);
  }

  template<typename... Rs, std::enable_if_t<conjunction<
      viewable_range<Rs>...
  >::value, int> = 0>
  constexpr auto operator()(Rs&&... rs) const {
    return ranges::zip_view<views::all_t<decltype((rs))>...>(rs...);
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::zip_niebloid zip{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ZIP_HPP_
