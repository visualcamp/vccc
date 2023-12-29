//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_ITERATOR_INDIRECT_BINARY_PREDICATE_HPP
#define VCCC_ITERATOR_INDIRECT_BINARY_PREDICATE_HPP

#include <type_traits>

#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/concepts/predicate.hpp"
#include "vccc/iterator/iter_common_reference_t.hpp"
#include "vccc/iterator/indirect_result_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename F,
    typename I1,
    typename I2,
    bool = conjunction<
               has_typename_type<iter_common_reference<I1>>,
               has_typename_type<iter_common_reference<I2>>
           >::value /* true */
>
struct indirect_binary_predicate_impl_2
    : conjunction<
          predicate<F&, iter_value_t<I1>&, iter_value_t<I2>&>,
          predicate<F&, iter_value_t<I1>&, iter_reference_t<I2>>,
          predicate<F&, iter_reference_t<I1>&, iter_value_t<I2>&>,
          predicate<F&, iter_reference_t<I1>&, iter_reference_t<I2>>,
          predicate<F&, iter_common_reference_t<I1>&, iter_common_reference_t<I2>>
      >{};

template<typename F, typename I1, typename I2>
struct indirect_binary_predicate_impl_2<F, I1, I2, false> : std::false_type {};


template<
    typename F,
    typename I1,
    typename I2,
    bool = conjunction<
               indirectly_readable<I1>,
               indirectly_readable<I2>,
               copy_constructible<F>
           >::value /* true */
>
struct indirect_binary_predicate_impl_1 : indirect_binary_predicate_impl_2<F, I1, I2> {};

template<typename F, typename I1, typename I2>
struct indirect_binary_predicate_impl_1<F, I1, I2, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator≤
/// @{


/**
@brief specifies that a callable type, when invoked with the result of dereferencing two `indirectly_readable` types,
satisfies `predicate`

The concept `%indirect_binary_predicate` specifies requirements for algorithms that call binary predicates as their
arguments. The key difference between this concept and `vccc::predicate` is that it is applied to the types that `I1`
and `I2` references, rather than `I1` and `I2` themselves.

@sa [std::indirect_binary_predicate](https://en.cppreference.com/w/cpp/iterator/indirect_binary_predicate)
 */
template<typename F, typename I1, typename I2>
struct indirect_binary_predicate : detail::indirect_binary_predicate_impl_1<F, I1, I2> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECT_BINARY_PREDICATE_HPP
