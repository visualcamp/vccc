//
// Created by yonggyulee on 2024/01/22.
//

#ifndef VCCC_RANGES_VIEWS_ENUMERATE_HPP
#define VCCC_RANGES_VIEWS_ENUMERATE_HPP

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/enumerate_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

class enumerate_adaptor_closure : public range_adaptor_closure<enumerate_adaptor_closure> {
 public:
  enumerate_adaptor_closure() = default;

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const {
    return enumerate_view<all_t<R>>(std::forward<R>(r));
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::enumerate_adaptor_closure enumerate{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ENUMERATE_HPP
