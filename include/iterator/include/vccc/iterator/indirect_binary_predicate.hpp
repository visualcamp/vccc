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
               has_typename_type<iter_value<I1>>,
               has_typename_type<iter_value<I2>>,
               has_typename_type<iter_reference<I1>>,
               has_typename_type<iter_reference<I2>>,
               has_typename_type<iter_common_reference<I1>>,
               has_typename_type<iter_common_reference<I2>>
           >::value
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
           >::value
>
struct indirect_binary_predicate_impl_1 : indirect_binary_predicate_impl_2<F, I1, I2> {};

template<typename F, typename I1, typename I2>
struct indirect_binary_predicate_impl_1<F, I1, I2, false> : std::false_type {};

} // namespace detail

/// @addtogroup iterator≤
/// @{

template<typename F, typename I1, typename I2>
struct indirect_binary_predicate : detail::indirect_binary_predicate_impl_1<F, I1, I2> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECT_BINARY_PREDICATE_HPP
