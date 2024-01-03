//
// Created by yonggyulee on 2024/01/03.
//

#ifndef VCCC_RANGES_DETAIL_SIMPLE_VIEW_HPP
#define VCCC_RANGES_DETAIL_SIMPLE_VIEW_HPP

#include <type_traits>

#include "vccc/concepts/same_as.hpp"
#include "vccc/ranges/iterator_t.hpp"
#include "vccc/ranges/sentinel_t.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/ranges/view.hpp"
#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename R, bool = conjunction<ranges::view<R>, ranges::range<const R>>::value /* true */>
struct simple_view
    : conjunction<
          same_as<ranges::iterator_t<R>, ranges::iterator_t<const R>>,
          same_as<ranges::sentinel_t<R>, ranges::sentinel_t<const R>>
      > {};

template<typename R>
struct simple_view<R, false> : std::false_type {};

} // namespace detail
} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_DETAIL_SIMPLE_VIEW_HPP
