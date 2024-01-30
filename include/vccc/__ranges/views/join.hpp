//
// Created by yonggyulee on 1/31/24.
//

#ifndef VCCC_RANGES_VIEWS_JOIN_HPP_
#define VCCC_RANGES_VIEWS_JOIN_HPP_

#include <type_traits>
#include <utility>

#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/join_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

class join_adaptor_object : public range_adaptor_closure<join_adaptor_object> {
 public:
  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const {
    return join_view<all_t<decltype((r))>>{std::forward<R>(r)};
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::join_adaptor_object join{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_JOIN_HPP_
