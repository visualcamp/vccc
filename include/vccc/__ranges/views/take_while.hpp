//
// Created by YongGyu Lee on 3/27/24.
//

#ifndef VCCC_RANGES_VIEWS_TAKE_WHILE_HPP_
#define VCCC_RANGES_VIEWS_TAKE_WHILE_HPP_

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__iterator/indirect_unary_predicate.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/range_adaptor.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/take_while_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct take_while_niebloid {
  template<typename R, typename Pred, std::enable_if_t<conjunction<
      has_typename_type<detail::all_t_impl<R>>,
      view<all_t<R>>,
      input_range<all_t<R>>,
      std::is_object<remove_cvref_t<Pred>>,
      indirect_unary_predicate<const remove_cvref_t<Pred>, iterator_t<all_t<R>>>
  >::value, int> = 0>
  constexpr auto operator()(R&& r, Pred&& pred) const {
    return take_while_view<all_t<R>, std::decay_t<Pred>>(std::forward<R>(r), std::forward<Pred>(pred));
  }

  template<typename Pred>
  constexpr auto operator()(Pred&& pred) const {
    return range_adaptor<take_while_niebloid, std::decay_t<Pred>>(std::forward<Pred>(pred));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::take_while_niebloid take_while{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_TAKE_WHILE_HPP_
