//
// Created by cosge on 2024-01-13.
//

#ifndef VCCC_ITERATOR_ITER_VAL_T_HPP_
#define VCCC_ITERATOR_ITER_VAL_T_HPP_

#include "vccc/__iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/__type_traits/has_typename_value_type.hpp"

namespace vccc {
namespace detail {

template<typename I, bool = has_typename_value_type< cxx20_iterator_traits<I> >::value /* true */>
struct iter_val_impl {
  using type = typename cxx20_iterator_traits<I>::value_type;
};
template<typename I>
struct iter_val_impl<I, false> {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename T>
struct iter_val : detail::iter_val_impl<T> {};

/// @sa [std::iter_val_t](https://en.cppreference.com/w/cpp/container/map/deduction_guides)
template<typename T>
using iter_val_t = typename iter_val<T>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_VAL_T_HPP_
