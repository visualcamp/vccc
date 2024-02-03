//
// Created by yonggyulee on 2/3/24.
//

#ifndef VCCC_RANGES_VIEWS_SPLIT_HPP
#define VCCC_RANGES_VIEWS_SPLIT_HPP

#include <type_traits>
#include <utility>

#include "split_view.hpp"
#include "vccc/__concepts/different_from.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/movable_box.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/single.hpp"
#include "vccc/__ranges/views/split_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<typename Pattern>
class split_adapter_closure : public range_adaptor_closure<split_adapter_closure<Pattern>> {
  template<typename R>
  using pattern_type =
      std::conditional_t<
          ranges::detail::same_with_range_value<R, Pattern>::value,
          single_view<range_value_t<R>>,
          all_t<Pattern>
      >;

  movable_box<Pattern> pattern_;

 public:
  template<typename T, std::enable_if_t<different_from<T, split_adapter_closure>::value, int> = 0>
  constexpr explicit split_adapter_closure(T&& pattern)
      : pattern_(std::forward<T>(pattern)) {}

  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const& {
    return split_view<all_t<R>, pattern_type<R>>(std::forward<R>(r), *pattern_);
  }

  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) && {
    return split_view<all_t<R>, pattern_type<R>>(std::forward<R>(r), std::move(*pattern_));
  }
};

struct split_niebloid {
  template<typename R, typename Pattern, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r, Pattern&& pattern) const {
    return vccc::ranges::make_split_view(std::forward<R>(r), std::forward<Pattern>(pattern));
  }

  template<typename Pattern>
  constexpr split_adapter_closure<std::remove_reference_t<Pattern>> operator()(Pattern&& pattern) const {
    return split_adapter_closure<std::remove_reference_t<Pattern>>(std::forward<Pattern>(pattern));
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::split_niebloid split{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_SPLIT_HPP
