//
// Created by yonggyulee on 2024/02/20.
//

#ifndef VCCC_RANGES_VIEWS_DROP_WHILE_HPP
#define VCCC_RANGES_VIEWS_DROP_WHILE_HPP

#include <type_traits>
#include <utility>

#include "vccc/__ranges/range_adaptor.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/drop_while_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct drop_while_niebloid {
  template<typename R, typename Pred, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r, Pred&& pred) const {
    return drop_while_view<all_t<R>, std::decay_t<Pred>>(std::forward<R>(r), std::forward<Pred>(pred));
  }

  template<typename Pred, std::enable_if_t<std::is_object<std::decay_t<Pred>>::value, int> = 0>
  constexpr auto operator()(Pred&& pred) const {
    return range_adaptor<drop_while_niebloid, std::decay_t<Pred>>(std::forward<Pred>(pred));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::drop_while_niebloid drop_while{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_DROP_WHILE_HPP
