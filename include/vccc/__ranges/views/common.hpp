//
// Created by YongGyu Lee on 2/8/24.
//

#ifndef VCCC_RANGES_VIEWS_COMMON_HPP
#define VCCC_RANGES_VIEWS_COMMON_HPP

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/common_range.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/common_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct common_adaptor_object : range_adaptor_closure<common_adaptor_object> {
 private:
  template<typename R>
  constexpr auto call(R&& r, std::true_type /* all and common */) const {
    return views::all(std::forward<R>(r));
  }
  template<typename R>
  constexpr auto call(R&& r, std::false_type /* all and common */) const {
    return common_view<all_t<R>>(std::forward<R>(r));
  }

 public:
  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const {
    return call(std::forward<R>(r), conjunction<is_invocable<all_adaptor_closure, R&&>, common_range<decltype((r))>>{});
  }
};

} // namespace detail

/// @addtogroup ranges
/// @{

/// @brief [RangeAdaptorObject](https://en.cppreference.com/w/cpp/named_req/RangeAdaptorObject).
VCCC_INLINE_OR_STATIC constexpr detail::common_adaptor_object common{};

/// @}

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_COMMON_HPP
