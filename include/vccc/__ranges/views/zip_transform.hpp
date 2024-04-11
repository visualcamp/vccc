//
// Created by YongGyu Lee on 4/11/24.
//

#ifndef VCCC_RANGES_VIEWS_ZIP_TRANSFORM_HPP_
#define VCCC_RANGES_VIEWS_ZIP_TRANSFORM_HPP_

#include <type_traits>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/invocable.hpp"
#include "vccc/__core/decay_copy.hpp"
#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__ranges/views/zip_transform_view.hpp"
#include "vccc/__ranges/range_adaptor.hpp"
#include "vccc/__ranges/range_adaptor_closure.hpp"
#include "vccc/__ranges/views/empty_view.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_invocable.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"

namespace vccc {
namespace ranges {
namespace views {
namespace detail {

struct zip_transform_niebloid {
 private:
  template<typename FD, bool = conjunction<copy_constructible<FD>, is_referencable<FD>>::value /* false */>
  struct check : std::false_type {};

  template<typename FD>
  struct check<FD, true>
      : conjunction<
          regular_invocable<FD&>,
          std::is_object<invoke_result_t<FD&>>
      > {};

 public:
  template<typename F, std::enable_if_t<check<std::decay_t<F&&>>::value, int> = 0>
  constexpr auto operator()(F&& f) const {
    using FD = std::decay_t<decltype(f)>;

    return ((void)f, vccc_decay_copy(views::empty<std::decay_t<invoke_result_t<FD&>>>));
  }

  template<typename F, typename... Rs>
  constexpr auto operator()(F&& f, Rs&&... rs) const {
    return zip_transform_view<std::decay_t<F>, views::all_t<Rs>...>(std::forward<F>(f), std::forward<Rs>(rs)...);
  }
};

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::zip_transform_niebloid zip_transform{};

} // namespace views
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_ZIP_TRANSFORM_HPP_
