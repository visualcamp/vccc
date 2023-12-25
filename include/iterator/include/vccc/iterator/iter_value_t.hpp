//
// Created by cosge on 2023-12-24.
//

#ifndef VCCC_ITERATOR_ITER_VALUE_T_HPP_
#define VCCC_ITERATOR_ITER_VALUE_T_HPP_

#include "vccc/iterator/iterator_traits/cxx20_iterator_traits.hpp"
#include "vccc/iterator/indirectly_readable_traits.hpp"
#include "vccc/type_traits/detail/requires_helper.hpp"
#include "vccc/type_traits/disjunction.hpp"
#include "vccc/type_traits/has_typename_value_type.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<
    typename T,
    bool = is_specialized_iterator_traits< cxx20_iterator_traits< remove_cvref_t<T> > >::value,
    bool = has_typename_value_type<indirectly_readable_traits<remove_cvref_t<T>>>::value
>
struct iter_value : requires_fail {};

template<typename T, bool v>
struct iter_value<T, true, v> : requires_pass {
  using type = typename cxx20_iterator_traits<remove_cvref_t<T>>::value_type;
};

template<typename T>
struct iter_value<T, false, true> : requires_pass {
  using type = typename indirectly_readable_traits<remove_cvref_t<T>>::value_type;
};

} // namespace detail

template<typename T>
using iter_value_t = typename detail::iter_value<T>::type;

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_VALUE_T_HPP_
