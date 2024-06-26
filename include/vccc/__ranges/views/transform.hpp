//
// Created by yonggyulee on 2023/01/12.
//

#ifndef VCCC_RANGES_VIEWS_TRANSFORM_HPP
#define VCCC_RANGES_VIEWS_TRANSFORM_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/invocable.hpp"
#include "vccc/__ranges/input_range.hpp"
#include "vccc/__ranges/range_adaptor.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/transform_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct transform_niebloid {
 private:
  template<typename R, typename F, bool = conjunction<input_range<R>, copy_constructible<F>>::value /* false */>
  struct check_range : std::false_type {};
  template<typename R, typename F>
  struct check_range<R, F, true>
      : conjunction<
            view<R>,
            std::is_object<F>,
            regular_invocable<F&, range_reference_t<R>>
        >{};

 public:
  template<typename R, typename F, std::enable_if_t<conjunction<
      viewable_range<R>,
      check_range<all_t<R>, std::decay_t<F>>
  >::value, int> = 0>
  constexpr auto operator()(R&& r, F&& fun) const {
    return transform_view<all_t<R>, std::decay_t<F>>(std::forward<R>(r), std::forward<F>(fun));
  }

  template<typename F, std::enable_if_t<conjunction<
      copy_constructible<std::decay_t<F>>,
      std::is_object<std::decay_t<F>>
  >::value, int> = 0>
  constexpr auto operator()(F&& fun) const {
    return range_adaptor<transform_niebloid, std::decay_t<F>>(std::forward<F>(fun));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::transform_niebloid transform{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_TRANSFORM_HPP
