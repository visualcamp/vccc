//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_ITERATOR_ITER_REFERENCE_T_HPP_
#define VCCC_ITERATOR_ITER_REFERENCE_T_HPP_

#include "vccc/concepts/dereferenceable.hpp"

namespace vccc {
namespace detail {

template<typename T, bool v = dereferenceable<T>::value>
struct iter_reference_impl {};

template<typename T>
struct iter_reference_impl<T, true> {
  using type = decltype(*std::declval<T&>());
};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename T>
struct iter_reference : detail::iter_reference_impl<T> {};

template<typename T>
using iter_reference_t = typename iter_reference<T>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_REFERENCE_T_HPP_
