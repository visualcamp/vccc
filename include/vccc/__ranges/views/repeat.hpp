//
// Created by yonggyulee on 2024/01/30.
//

#ifndef VCCC_RANGES_VIEWS_REPEAT_HPP
#define VCCC_RANGES_VIEWS_REPEAT_HPP

#include <type_traits>

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/move_constructible.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__concepts/semiregular.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/views/repeat_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/is_integer_like.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct repeat_niebloid {
  template<typename W, std::enable_if_t<conjunction<
      move_constructible< remove_cvref_t<W> >,
      std::is_object< remove_cvref_t<W> >
  >::value, int> = 0>
  constexpr auto operator()(W&& value) const {
    using RV = repeat_view<remove_cvref_t<W>>;
    return RV(std::forward<W>(value));
  }

  template<typename W, typename Bound, std::enable_if_t<conjunction<
      move_constructible< remove_cvref_t<W> >,
      std::is_object< remove_cvref_t<W> >,
      disjunction<
          is_integer_like< remove_cvref_t<Bound> >,
          same_as< remove_cvref_t<Bound>, unreachable_sentinel_t >
      >
  >::value, int> = 0>
  constexpr auto operator()(W&& value, Bound&& bound) const {
    using RV = repeat_view<remove_cvref_t<W>, remove_cvref_t<Bound>>;
    return RV(std::forward<W>(value), std::forward<Bound>(bound));
  }
};

} // namespace detail


/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::repeat_niebloid repeat{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_REPEAT_HPP
