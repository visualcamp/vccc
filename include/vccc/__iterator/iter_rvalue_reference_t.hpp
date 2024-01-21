//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_ITERATOR_ITER_RVALUE_REFERENCE_T_HPP_
#define VCCC_ITERATOR_ITER_RVALUE_REFERENCE_T_HPP_

#include <type_traits>

#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__iterator/iter_move.hpp"

namespace vccc {
namespace detail {

template<typename T, bool v = dereferenceable<T>::value, typename = void>
struct iter_rvalue_reference_impl {};

template<typename T>
struct iter_rvalue_reference_impl<T, true, void_t<decltype(ranges::iter_move(std::declval<T&>()))>> {
  using type = decltype(ranges::iter_move(std::declval<T&>()));
};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename T>
struct iter_rvalue_reference : detail::iter_rvalue_reference_impl<T> {};

template<typename T>
using iter_rvalue_reference_t = typename iter_rvalue_reference<T>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_RVALUE_REFERENCE_T_HPP_
