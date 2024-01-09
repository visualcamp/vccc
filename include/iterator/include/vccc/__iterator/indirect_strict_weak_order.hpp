//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_ITERATOR_INDIRECT_STRICT_WEAK_ORDER_HPP
#define VCCC_ITERATOR_INDIRECT_STRICT_WEAK_ORDER_HPP

#include <type_traits>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/strict_weak_order.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/iter_common_reference_t.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename F,
    typename I1,
    typename I2,
    bool = conjunction<
               indirectly_readable<I1>,
               indirectly_readable<I2>,
               copy_constructible<F>,
               has_typename_type<iter_common_reference<I1>>,
               has_typename_type<iter_common_reference<I2>>
           >::value /* false */
>
struct indirect_strict_weak_order_impl_1 : std::false_type {};


template<typename F, typename I1, typename I2>
struct indirect_strict_weak_order_impl_1<F, I1, I2, true>
    : conjunction<
          strict_weak_order<F&, iter_value_t<I1>&,           iter_value_t<I2>&>,
          strict_weak_order<F&, iter_value_t<I1>&,           iter_reference_t<I2>&>,
          strict_weak_order<F&, iter_reference_t<I1>&,       iter_value_t<I2>&>,
          strict_weak_order<F&, iter_reference_t<I1>&,       iter_reference_t<I2>&>,
          strict_weak_order<F&, iter_common_reference_t<I1>, iter_common_reference_t<I2>>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that a callable type, when invoked with the result of dereferencing two `indirectly_readable` types,
satisfies `strict_weak_order`

The concept `%indirect_strict_weak_order` specifies requirements for algorithms that call strict weak orders as their
arguments. The key difference between this concept and `vccc::strict_weak_order` is that it is applied to the types that
`I1` and `I2` references, rather than `I1` and `I2` themselves.

@sa [std::indirect_strict_weak_order](https://en.cppreference.com/w/cpp/iterator/indirect_strict_weak_order)
 */
template<typename F, typename I1, typename I2 = I1>
struct indirect_strict_weak_order : detail::indirect_strict_weak_order_impl_1<F, I1, I2> {};

/// @} iterator

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECT_STRICT_WEAK_ORDER_HPP
