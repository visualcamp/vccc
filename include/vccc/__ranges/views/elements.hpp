//
// Created by yonggyulee on 2024/01/22.
//

#ifndef VCCC_RANGES_VIEWS_ELEMENTS_HPP
#define VCCC_RANGES_VIEWS_ELEMENTS_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/views/all.hpp"
#include "vccc/__ranges/views/elements_view.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<std::size_t N>
class elements_adaptor_object : public range_adaptor_closure<elements_adaptor_object<N>> {
 public:
  constexpr elements_adaptor_object() = default;

  template<typename R, std::enable_if_t<viewable_range<R>::value, int> = 0>
  constexpr auto operator()(R&& r) const {
    return elements_view<all_t<R>, N>(std::forward<R>(r));
  }
};

} // namespace detail

template<std::size_t N>
VCCC_INLINE_OR_STATIC constexpr detail::elements_adaptor_object<N> elements{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ELEMENTS_HPP
