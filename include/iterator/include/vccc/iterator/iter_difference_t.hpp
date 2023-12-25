//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_ITER_DIFFERENCE_T_HPP_
#define VCCC_ITERATOR_ITER_DIFFERENCE_T_HPP_

#include "vccc/iterator/incrementable_traits.hpp"
#include "vccc/iterator/iterator_traits/forward_declare.hpp"
#include "vccc/type_traits/has_typename_difference_type.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {

template<
    typename T,
    bool = detail::is_specialized_iterator_traits< cxx20_iterator_traits< remove_cvref_t<T> > >::value,
    bool = has_typename_difference_type<incrementable_traits<remove_cvref_t<T>>>::value
>
struct iter_difference {};

template<typename T, bool v>
struct iter_difference<T, true, v> {
  using type = typename cxx20_iterator_traits<remove_cvref_t<T>>::difference_type;
};

template<typename T>
struct iter_difference<T, false, true> {
  using type = typename incrementable_traits<remove_cvref_t<T>>::difference_type;
};

/// @addtogroup iterator
/// @{

/**
 * @brief Computes the \a difference type of `T`
 *
 * Computes the \a difference type of `T`. If `cxx20_iterator_traits<remove_cvref_t<T>>` is not specialized,
 * then `iter_difference_t<T>` is `incrementable_traits<remove_cvref_t<T>>::difference_type`.
 * Otherwise, it is `cxx20_iterator_traits<remove_cvref_t<T>>::difference_type`
 */
template<typename T>
using iter_difference_t = typename iter_difference<T>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_DIFFERENCE_T_HPP_
