//
// Created by cosge on 2023-12-02.
//

#ifndef VCCC_ITERATOR_ITER_REFERENCE_T_HPP_
#define VCCC_ITERATOR_ITER_REFERENCE_T_HPP_

#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/type_traits/detail/requires_helper.hpp"

namespace vccc {

template<typename T, bool v = dereferenceable<T>::value>
struct iter_reference : detail::requires_fail {};

template<typename T>
struct iter_reference<T, true> : detail::requires_pass {
  using type = decltype(*std::declval<T&>());
};

template<typename T>
using iter_reference_t = typename iter_reference<T>::type;

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_REFERENCE_T_HPP_
