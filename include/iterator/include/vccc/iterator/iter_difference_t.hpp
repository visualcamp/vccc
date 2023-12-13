//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_ITER_DIFFERENCE_T_HPP_
#define VCCC_ITERATOR_ITER_DIFFERENCE_T_HPP_

#include <iterator>

#include "vccc/type_traits/is_complete.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/iterator/incrementable_traits.hpp"

namespace vccc {
namespace detail {

template<
    typename T,
    bool = is_complete< std::iterator_traits< remove_cvref_t<T> > >::value>
struct iter_difference;

template<typename T>
struct iter_difference<T, true> {
  using type = typename std::iterator_traits<remove_cvref_t<T>>::difference_type;
};

template<typename T>
struct iter_difference<T, false> {
  using type = typename incrementable_traits<remove_cvref_t<T>>::difference_type;
};

} // namespace detail

template<typename T>
using iter_difference_t = typename detail::iter_difference<T>::type;

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_DIFFERENCE_T_HPP_
