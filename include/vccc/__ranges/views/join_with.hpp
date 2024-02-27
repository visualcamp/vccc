//
// Created by yonggyulee on 2/13/24.
//

#ifndef VCCC_RANGES_VIEWS_JOIN_WITH_HPP_
#define VCCC_RANGES_VIEWS_JOIN_WITH_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__concepts/different_from.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/range_reference_t.hpp"
#include "vccc/__ranges/range_value_t.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/join_with_view.hpp"
#include "vccc/__ranges/views/single.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<typename Pattern>
class join_with_adaptor_object : public range_adaptor_closure<join_with_adaptor_object<Pattern>> {
 public:
  template<typename U, std::enable_if_t<different_from<join_with_adaptor_object, U>::value, int> = 0>
  constexpr explicit join_with_adaptor_object(U&& pattern)
      : pattern_(std::forward<U>(pattern)) {}

  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const& {
    return join_with_view<all_t<R>, all_t<const Pattern&>>{std::forward<R>(r), pattern_};
  }

  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) && {
    return join_with_view<all_t<R>, all_t<Pattern>>{std::forward<R>(r), std::move(pattern_)};
  }

 private:
  Pattern pattern_;
};

struct join_with_niebloid {
  template<typename R, typename Pattern, std::enable_if_t<conjunction<
      viewable_range<R>,
      different_from<Pattern, range_value_t<range_reference_t<R>>>
  >::value, int> = 0>
  constexpr auto operator()(R&& r, Pattern&& pattern) const {
    return join_with_view<all_t<R>, all_t<Pattern>>{std::forward<R>(r), std::forward<Pattern>(pattern)};
  }

  template<typename R, std::enable_if_t<conjunction<
      viewable_range<R>
  >::value, int> = 0>
  constexpr auto operator()(R&& r, range_value_t<range_reference_t<R>> pattern) const {
    return join_with_view<all_t<R>, single_view<range_value_t<range_reference_t<R>>>>{
        std::forward<R>(r), std::move(pattern)};
  }

  template<typename Pattern>
  constexpr auto operator()(Pattern&& pattern) const {
    return join_with_adaptor_object<std::remove_reference_t<Pattern>>{std::forward<Pattern>(pattern)};
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

VCCC_INLINE_OR_STATIC constexpr detail::join_with_niebloid join_with{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_JOIN_WITH_HPP_
