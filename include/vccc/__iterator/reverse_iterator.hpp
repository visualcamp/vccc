//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ITERATOR_REVERSE_ITERATOR_HPP_
#define VCCC_ITERATOR_REVERSE_ITERATOR_HPP_


#include <iterator>
#include <type_traits>

#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__iterator/indirectly_swappable.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/iter_rvalue_reference_t.hpp"
#include "vccc/__iterator/iter_swap.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__iterator/random_access_iterator.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__type_traits/negation.hpp"

namespace vccc {

/// @addtogroup iterator
/// @{

template<typename I1, typename I2>
struct disable_sized_sentinel_for<std::reverse_iterator<I1>, std::reverse_iterator<I2>>
    : negation<sized_sentinel_for<I1, I2>> {};

// iterator_traits
#if __cplusplus < 202002L

template<typename Iter>
struct cxx20_iterator_traits<std::reverse_iterator<Iter>> {
  using iterator_concept = std::conditional_t<
      random_access_iterator<Iter>::value,
      random_access_iterator_tag,
      bidirectional_iterator_tag
  >;
  using iterator_category = std::conditional_t<
      derived_from<typename cxx20_iterator_traits<Iter>::iterator_category, random_access_iterator_tag>::value,
      random_access_iterator_tag,
      typename cxx20_iterator_traits<Iter>::iterator_category
  >;
  using value_type = iter_value_t<Iter>;
  using difference_type = iter_difference_t<Iter>;
  using pointer = typename cxx20_iterator_traits<Iter>::pointer;
  using reference = iter_reference_t<Iter>;
};

namespace detail {

template<typename Iter>
struct is_primary_iterator_traits<cxx20_iterator_traits<std::reverse_iterator<Iter>>> : std::true_type {};

} // namespace detail

#endif // __cplusplus < 202002L

/// @}

} // namespace vccc

// iter_move, iter_swap
#if __cplusplus < 202002L

namespace std {

template<typename Iter>
constexpr vccc::iter_rvalue_reference_t<Iter> iter_move(const std::reverse_iterator<Iter>& i)
    noexcept(
        std::is_nothrow_copy_constructible<Iter>::value &&
        noexcept(vccc::ranges::iter_move(--std::declval<Iter&>()))
    )
{
  auto tmp = i.base();
  return vccc::ranges::iter_move(--tmp);
}

template<typename Iter, typename Iter2, std::enable_if_t<vccc::indirectly_swappable<Iter2, Iter>::value, int> = 0>
constexpr void iter_swap(const reverse_iterator<Iter>& x, const reverse_iterator<Iter2>& y)
    noexcept(
        std::is_nothrow_copy_constructible<Iter>::value &&
        std::is_nothrow_copy_constructible<Iter2>::value &&
        noexcept(vccc::ranges::iter_swap(--std::declval<Iter&>(), --std::declval<Iter2&>()))
    )
{
  auto tmp_x = x.base();
  auto tmp_y = y.base();
  vccc::ranges::iter_swap(--tmp_x, --tmp_y);
}

} // namespace std

#endif // __cplusplus < 202002L

#endif // VCCC_ITERATOR_REVERSE_ITERATOR_HPP_
