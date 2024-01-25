//
// Created by yonggyulee on 1/24/24.
//

#ifndef VCCC_RANGES_VIEWS_FILTER_HPP
#define VCCC_RANGES_VIEWS_FILTER_HPP

#include <type_traits>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/views/filter_view.hpp"
#include "vccc/__ranges/viewable_range.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

template<typename Pred>
class filter_adaptor_closure : public range_adaptor_closure<filter_adaptor_closure<Pred>>, movable_box<Pred> {
  using pred = movable_box<Pred>;
 public:
  explicit filter_adaptor_closure(Pred f) : pred(f) {}

  template<typename R, std::enable_if_t<range<R>::value, int> = 0>
  constexpr filter_view<all_t<R>, Pred>
  operator()(R&& r) const {
    return filter_view<all_t<R>, Pred>(std::forward<R>(r), *static_cast<const pred&>(*this));
  }
};

template<typename R, typename Pred, bool = viewable_range<R>::value /* false */>
struct check_filter_call : std::false_type {};
template<typename R, typename Pred>
struct check_filter_call<R, Pred, true>
    : conjunction<
          input_range<R>,
          indirect_unary_predicate<Pred, iterator_t<R>>,
          std::is_object<R>
      > {};

struct filter_adaptor {
  template<typename R, class Pred, std::enable_if_t<check_filter_call<R, Pred>::value, int> = 0>
  constexpr auto operator()(R&& r, Pred&& pred) const {
    return filter_view<all_t<R>, Pred>(std::forward<R>(r), std::forward<Pred>(pred));
  }

  template<typename Pred>
  constexpr filter_adaptor_closure<std::decay_t<Pred>> operator()(Pred&& pred) const {
    return filter_adaptor_closure<std::decay_t<Pred>>(std::forward<Pred>(pred));
  }

};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::filter_adaptor filter{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_FILTER_HPP
