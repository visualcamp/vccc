//
// Created by cosge on 2024-01-13.
//

#ifndef VCCC_ITERATOR_ITER_KEY_T_HPP_
#define VCCC_ITERATOR_ITER_KEY_T_HPP_

#include <tuple>
#include <type_traits>

#include "vccc/__iterator/iter_val_t.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/is_complete.hpp"

namespace vccc {
namespace detail {

template<typename V, bool = is_complete<std::tuple_element<0, V>>::value /* true */>
struct iter_key_impl_2 {
  using type = std::remove_const_t< std::tuple_element_t<0, V> >;
};
template<typename V>
struct iter_key_impl_2<V, false> {};

template<typename I, bool = has_typename_type< iter_val<I> >::value /* true */>
struct iter_key_impl : iter_key_impl_2<iter_val_t<I>> {};
template<typename I>
struct iter_key_impl<I, false> {};

} // namespace detail

/// @addtogroup iterator
/// @{

template<typename T>
struct iter_key : detail::iter_key_impl<T> {};

/// @sa [std::iter_key_t](https://en.cppreference.com/w/cpp/container/map/deduction_guides)
template<typename T>
using iter_key_t = typename iter_key<T>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_KEY_T_HPP_
