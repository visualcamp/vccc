//
// Created by yonggyulee on 1/24/24.
//

#ifndef VCCC_RANGES_VIEWS_FILTER_HPP
#define VCCC_RANGES_VIEWS_FILTER_HPP

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/range_adaptor.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/views/filter_view.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct filter_niebloid {
  template<typename R, typename Pred, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r, Pred&& pred) const {
    return filter_view<all_t<R>, remove_cvref_t<Pred>>(std::forward<R>(r), std::forward<Pred>(pred));
  }

  template<typename Pred>
  constexpr auto operator()(Pred&& pred) const {
    return range_adaptor<filter_niebloid, std::decay_t<Pred>>{std::forward<Pred>(pred)};
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::filter_niebloid filter{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_FILTER_HPP
