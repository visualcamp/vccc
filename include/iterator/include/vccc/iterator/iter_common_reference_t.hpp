//
// Created by yonggyulee on 2023/12/28.
//

#ifndef VCCC_ITERATOR_ITER_COMMON_REFERENCE_T_HPP
#define VCCC_ITERATOR_ITER_COMMON_REFERENCE_T_HPP

#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/type_traits/common_reference.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename T,
    bool = has_typename_type<
               common_reference< iter_reference_t<T>,
                                 iter_value_t<T>&    >
           >::value /* true */
>
struct iter_common_reference_impl_2 {
  using type = common_reference_t<iter_reference_t<T>, iter_value_t<T>&>;
};
template<typename T>
struct iter_common_reference_impl_2<T, false> {};

template<typename T, bool = indirectly_readable<T>::value /* true */>
struct iter_common_reference_impl_1 : iter_common_reference_impl_2<T> {};
template<typename T>
struct iter_common_reference_impl_1<T, false> {};

} // namespace detail

template<typename T>
struct iter_common_reference : detail::iter_common_reference_impl_1<T> {};

/// @addtogroup iterator
/// @{

/// @brief Computes the <em>common reference type</em> of `T`.
/// This is the common reference type between its reference type and an lvalue reference to its value type.
template<typename T>
using iter_common_reference_t = typename iter_common_reference<T>::type;

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITER_COMMON_REFERENCE_T_HPP
