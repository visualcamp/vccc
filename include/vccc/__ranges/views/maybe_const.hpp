//
// Created by YongGyu Lee on 1/26/24.
//

#ifndef VCCC_RANGES_VIEWS_MAYBE_CONST_HPP
#define VCCC_RANGES_VIEWS_MAYBE_CONST_HPP

#include <type_traits>

namespace vccc {
namespace ranges {
namespace detail {

template<bool Const, typename V>
using maybe_const = std::conditional_t<Const, const V, V>;

} // namespace detail
} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_VIEWS_MAYBE_CONST_HPP
