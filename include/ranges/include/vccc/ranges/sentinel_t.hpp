//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_RANGES_SENTINEL_T_HPP_
#define VCCC_RANGES_SENTINEL_T_HPP_

#include <type_traits>

#include "vccc/iterator/sized_sentinel_for.hpp"
#include "vccc/ranges/end.hpp"
#include "vccc/ranges/range.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace ranges {

template<typename R, bool = ranges::range<R>::value, typename = void>
struct sentinel {};

template<typename R>
struct sentinel<R, true, void_t<decltype( ranges::end(std::declval<R&>()) )>> {
  using type = decltype(ranges::end(std::declval<R&>()));
};

template<typename R>
using sentinel_t = typename ranges::sentinel<R>::type;

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_SENTINEL_T_HPP_
