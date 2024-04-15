//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ITERATOR_DETAIL_ITER_EXCHANGE_MOVE_HPP_
#define VCCC_ITERATOR_DETAIL_ITER_EXCHANGE_MOVE_HPP_

#include <type_traits>

#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_value_t.hpp"

namespace vccc {
namespace detail {

template<typename T, typename U>
constexpr iter_value_t<std::remove_reference_t<T>> iter_exchange_move(T&& x, U&& y)
    noexcept(noexcept(iter_value_t<std::remove_reference_t<T>>(ranges::iter_move(x))))
{
  iter_value_t<std::remove_reference_t<T>> tmp(ranges::iter_move(x));
  *x = ranges::iter_move(y);
  return tmp;
}

} // namespace detail
} // namespace vccc

#endif // VCCC_ITERATOR_DETAIL_ITER_EXCHANGE_MOVE_HPP_
