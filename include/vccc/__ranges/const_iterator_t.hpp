//
// Created by cosge on 2024-01-27.
//

#ifndef VCCC_RANGES_CONST_ITERATOR_T_HPP_
#define VCCC_RANGES_CONST_ITERATOR_T_HPP_

#include <type_traits>

#include "vccc/__ranges/cbegin.hpp"
#include "vccc/__ranges/range.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__type_traits/void_t.hpp"

namespace vccc {
namespace ranges {
namespace detail {

template<typename T, bool = range<T>::value, typename = void>
struct const_iterator_impl {};

template<typename T>
struct const_iterator_impl<T, true, void_t<decltype( ranges::cbegin(std::declval<T&>()) )>> {
  using type = decltype(ranges::cbegin(std::declval<T&>()));
};

} // namespace detail

/// @addtogroup ranges
/// @{

template<typename T>
struct const_iterator : detail::const_iterator_impl<T> {};

template<typename T>
using const_iterator_t = typename const_iterator<T>::type;

/// @}

} // namespace ranges
} // namespace vccc

#endif // VCCC_RANGES_CONST_ITERATOR_T_HPP_
