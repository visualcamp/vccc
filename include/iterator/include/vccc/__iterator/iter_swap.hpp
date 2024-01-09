//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_ITERATOR_ITER_SWAP_HPP
#define VCCC_ITERATOR_ITER_SWAP_HPP

#include <algorithm>

#include "vccc/core.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace ranges {
namespace detail {

struct iter_swap_niebloid {


};

template<typename X, typename Y>
constexpr iter_value_t<X>
iter_exchange_move(X&& x, Y&& y) noexcept(noexcept( iter_value_t<X>(ranges::iter_move(x)) ) &&
                                          noexcept( *x = ranges::iter_move(y)             ))
{
  vccc::iter_value_t<X> old(vccc::ranges::iter_move(x));
  *x = vccc::ranges::iter_move(y);
  return old;
}

} // namespace detail

VCCC_INLINE_OR_STATIC constexpr detail::iter_swap_niebloid iter_swap{};

} // namespace ranges
} // namespace vccc

#endif // VCCC_ITERATOR_ITER_SWAP_HPP
