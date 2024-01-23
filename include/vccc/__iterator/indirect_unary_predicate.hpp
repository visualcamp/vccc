//
// Created by yonggyulee on 2024/01/12.
//

#ifndef VCCC_ITERATOR_INDIRECT_UNARY_PREDICATE_HPP
#define VCCC_ITERATOR_INDIRECT_UNARY_PREDICATE_HPP

#include <type_traits>

#include "vccc/__concepts/copy_constructible.hpp"
#include "vccc/__concepts/predicate.hpp"
#include "vccc/__iterator/indirectly_readable.hpp"
#include "vccc/__iterator/iter_common_reference_t.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__iterator/iter_value_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<typename F, typename I, bool = conjunction<indirectly_readable<I>, copy_constructible<F>>::value /* false */>
struct indirect_unary_predicate_impl : std::false_type {};

template<typename F, typename I>
struct indirect_unary_predicate_impl<F, I, true>
    : conjunction<
          predicate<F&, iter_value_t<I>>,
          predicate<F&, iter_reference_t<I>>,
          predicate<F&, iter_common_reference_t<I>>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{


/**
@brief specifies that a callable type, when invoked with the result of dereferencing an `indirectly_readable` type,
satisfies `predicate`

The concept `%indirect_unary_predicate` specifies requirements for algorithms that call unary predicates as their
arguments. The key difference between this concept and `vccc::predicate` is that it is applied to the type that `I`
references, rather than `I` itself.

@sa [std::indirect_unary_predicate](https://en.cppreference.com/w/cpp/iterator/indirect_unary_predicate)
 */
template<typename F, typename I>
struct indirect_unary_predicate : detail::indirect_unary_predicate_impl<F, I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECT_UNARY_PREDICATE_HPP
