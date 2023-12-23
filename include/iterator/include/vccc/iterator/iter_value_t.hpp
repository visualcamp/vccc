//
// Created by cosge on 2023-12-24.
//

#ifndef VCCC_ITERATOR_ITER_VALUE_T_HPP_
#define VCCC_ITERATOR_ITER_VALUE_T_HPP_

#include "vccc/iterator/indirectly_readable_traits.hpp"
#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/type_traits/has_typename_value_type.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {

template<
    typename T,
    bool = has_typename_value_type< std::iterator_traits< remove_cvref_t<T> > >::value
>
struct iter_value;

template<typename T>
struct iter_value<T, true> {
  using type = typename std::iterator_traits<remove_cvref_t<T>>::value_type;
};

template<typename T>
struct iter_value<T, false> {
  using type = typename indirectly_readable_traits<remove_cvref_t<T>>::value_type;
};

template<typename T>
using iter_value_t = typename iter_reference<T>::type;

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_VALUE_T_HPP_
