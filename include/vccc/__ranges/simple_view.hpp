//
// Created by yonggyulee on 2024/01/03.
//

#ifndef VCCC_RANGES_SIMPLE_VIEW_HPP
#define VCCC_RANGES_SIMPLE_VIEW_HPP

#include <type_traits>

#include "vccc/__concepts/same_as.hpp"
#include "vccc/__ranges/iterator_t.hpp"
#include "vccc/__ranges/sentinel_t.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__ranges/view.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace ranges {

template<typename R, bool = conjunction<view<R>, range<const R>>::value /* true */>
struct simple_view
    : conjunction<
          same_as<iterator_t<R>, iterator_t<const R>>,
          same_as<sentinel_t<R>, sentinel_t<const R>>
      > {};

template<typename R>
struct simple_view<R, false> : std::false_type {};

} // namespace vccc
} // namespace ranges

#endif // VCCC_RANGES_SIMPLE_VIEW_HPP
