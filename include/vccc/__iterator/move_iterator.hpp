//
// Created by YongGyu Lee on 4/15/24.
//

#ifndef VCCC_ITERATOR_MOVE_ITERATOR_HPP_
#define VCCC_ITERATOR_MOVE_ITERATOR_HPP_

#include <iterator>
#include <type_traits>

#include "vccc/__core/constexpr.hpp"
#include "vccc/__concepts/derived_from.hpp"
#include "vccc/__iterator/bidirectional_iterator.hpp"
#include "vccc/__iterator/forward_iterator.hpp"
#include "vccc/__iterator/indirectly_swappable.hpp"
#include "vccc/__iterator/iter_difference_t.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_rvalue_reference_t.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__iterator/iterator_tag.hpp"
#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__iterator/sized_sentinel_for.hpp"
#include "vccc/__iterator/move_sentinel.hpp"
#include "vccc/__iterator/sentinel_for.hpp"
#include "vccc/__iterator/random_access_iterator.hpp"
#include "vccc/__type_traits/negation.hpp"


namespace vccc {

template<typename Iterator1, typename Iterator2>
struct disable_sized_sentinel_for<std::move_iterator<Iterator1>, std::move_iterator<Iterator2>>
    : negation<sized_sentinel_for<Iterator1, Iterator2>> {};

#if __cplusplus < 202302L

namespace detail {

template<typename Iter, bool = has_typename_iterator_category<Iter>::value /* false */>
struct move_iterator_category {};

template<typename Iter>
struct move_iterator_category<Iter, true> {
  using iterator_category = std::conditional_t<
      derived_from<typename cxx20_iterator_traits<Iter>::iterator_category, random_access_iterator_tag>::value, random_access_iterator_tag,
      typename cxx20_iterator_traits<Iter>::iterator_category
  >;
};

} // namespace detail

template<typename Iter>
struct cxx20_iterator_traits<std::move_iterator<Iter>>
    : detail::move_iterator_category<Iter>
{
  using iterator_concept =
      std::conditional_t<
          random_access_iterator<Iter>::value, random_access_iterator_tag,
      std::conditional_t<
          bidirectional_iterator<Iter>::value, bidirectional_iterator_tag,
      std::conditional_t<
          forward_iterator<Iter>::value, forward_iterator_tag,
          input_iterator_tag
      >>>;
  using value_type = iter_value_t<Iter>;
  using difference_type = iter_difference_t<Iter>;
  using pointer = Iter;
  using reference = iter_rvalue_reference_t<Iter>;
};

namespace detail {

template<typename Iter>
struct is_primary_iterator_traits<std::move_iterator<Iter>> : std::true_type {};

} // namespace detail

#endif // __cplusplus < 202302L

} // namespace vccc


#if __cplusplus < 202002L

namespace std {

template<typename Iter>
constexpr vccc::iter_rvalue_reference_t<Iter> iter_move(const move_iterator<Iter>& i)
    noexcept(noexcept(vccc::ranges::iter_move(i.base())))
{
  return vccc::ranges::iter_move(i.base());
}

template<typename Iter, typename Iter2, std::enable_if_t<vccc::indirectly_swappable<Iter, Iter2>::value, int> = 0>
constexpr void iter_swap(const move_iterator<Iter>& x, const move_iterator<Iter2>& y)
    noexcept(noexcept(vccc::ranges::iter_swap(x.base(), y.base())))
{
  vccc::ranges::iter_swap(x.base(), y.base());
}

} // namespace std

#endif

#endif // VCCC_ITERATOR_MOVE_ITERATOR_HPP_
