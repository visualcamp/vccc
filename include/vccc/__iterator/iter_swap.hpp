//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_ITERATOR_ITER_SWAP_HPP
#define VCCC_ITERATOR_ITER_SWAP_HPP

#include <type_traits>
#include <utility>

#include "vccc/__core/inline_or_static.hpp"
#include "vccc/__concepts/swappable_with.hpp"
#include "vccc/__iterator/indirectly_movable_storable.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/iter_move.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/disjunction.hpp"
#include "vccc/__type_traits/negation.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace ranges {
namespace detail_iter_swap {

template<typename T, typename U>
void iter_swap(T, U) = delete;

template<typename T, typename U>
constexpr auto test_iter_swap(int) -> decltype(iter_swap(std::declval<T>(), std::declval<U>()), std::true_type{});

template<typename T, typename U>
constexpr auto test_iter_swap(...) -> std::false_type;

template<typename T, typename U, bool = disjunction<
    std::is_class<remove_cvref_t<T>>, std::is_enum<remove_cvref_t<T>>,
    std::is_class<remove_cvref_t<U>>, std::is_enum<remove_cvref_t<U>>
  >::value>
struct unqual_iter_swap : decltype(test_iter_swap<T, U>(0)) {};

template<typename T, typename U>
struct unqual_iter_swap<T, U, true> : decltype(test_iter_swap<T, U>(0)) {};

template<typename I1, typename I2>
struct read_iter_swap_2 : swappable_with<iter_reference_t<I1>, iter_reference_t<I2>> {};

template<typename I1, typename I2, bool = conjunction<indirectly_readable<I1>, indirectly_readable<I2>>::value /* true */>
struct read_iter_swap : read_iter_swap_2<I1, I2> {};
template<typename I1, typename I2>
struct read_iter_swap<I1, I2, false> : std::false_type {};

struct iter_swap_niebloid {
  template<typename I1, typename I2, std::enable_if_t<unqual_iter_swap<I1, I2>::value, int> = 0>
  constexpr void operator()(I1&& i1, I2&& i2) const
      noexcept(noexcept(iter_swap(std::forward<I1>(i1), std::forward<I2>(i2))))
  {
    (void)iter_swap(std::forward<I1>(i1), std::forward<I2>(i2));
  }

  template<typename I1, typename I2, std::enable_if_t<conjunction<
      negation< unqual_iter_swap<I1, I2> >,
      read_iter_swap<I1, I2>
  >::value, int> = 0>
  constexpr void operator()(I1&& i1, I2&& i2) const
      noexcept(noexcept(ranges::swap(*i1, *i2)))
  {
    ranges::swap(*std::forward<I1>(i1), *std::forward<I2>(i2));
  }

  template<typename I1, typename I2, std::enable_if_t<conjunction<
      negation< unqual_iter_swap<I1, I2> >,
      negation< read_iter_swap<I1, I2> >,
      indirectly_movable_storable<I1, I2>,
      indirectly_movable_storable<I2, I1>
  >::value, int> = 0>
  constexpr void operator()(I1&& x, I2&& y) const
    noexcept(noexcept( iter_value_t<I1>(ranges::iter_move(x)) ) &&
             noexcept( *x = ranges::iter_move(y) ) &&
             noexcept( *std::forward<I2>(y) = std::declval<iter_value_t<I1>>() ))
  {
    iter_value_t<I1> old(ranges::iter_move(x));
    *x = ranges::iter_move(y);
    *std::forward<I2>(y) = std::move(old);
  }
};

} // namespace detail_iter_swap

inline namespace niebloid {

/// @addtogroup iterator
/// @{

VCCC_INLINE_OR_STATIC constexpr detail_iter_swap::iter_swap_niebloid iter_swap{};

/// @}

} // inline namespace niebloid

} // namespace ranges
} // namespace vccc

#endif // VCCC_ITERATOR_ITER_SWAP_HPP
