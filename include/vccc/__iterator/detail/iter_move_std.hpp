//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_DETAIL_ITER_MOVE_STD_HPP_
#define VCCC_ITERATOR_DETAIL_ITER_MOVE_STD_HPP_

#if __cplusplus < 202002L

#include <iterator>
#include <type_traits>

#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_rvalue_reference_t.hpp"

namespace std {

template<typename Iter>
constexpr vccc::iter_rvalue_reference_t<Iter>
iter_move(const reverse_iterator<Iter>& i)
    noexcept(
      is_nothrow_copy_constructible<Iter>::value &&
      noexcept(vccc::ranges::iter_move(--declval<Iter&>()))
    )
{
  auto tmp = i.base();
  return vccc::ranges::iter_move(--tmp);
}

template<typename Iter>
constexpr vccc::iter_rvalue_reference_t<Iter>
iter_move(const move_iterator<Iter>& i)
    noexcept(noexcept(vccc::ranges::iter_move(--declval<Iter&>())))
{
  return vccc::ranges::iter_move(i.base());
}

} // namespace std

#endif

#endif // VCCC_ITERATOR_DETAIL_ITER_MOVE_STD_HPP_
