//
// Created by yonggyulee on 2023/12/29.
//

#ifndef VCCC_ITERATOR_INDIRECT_UNARY_INVOCABLE_HPP
#define VCCC_ITERATOR_INDIRECT_UNARY_INVOCABLE_HPP

#include <type_traits>

#include "vccc/concepts/common_reference_with.hpp"
#include "vccc/concepts/copy_constructible.hpp"
#include "vccc/concepts/invocable.hpp"
#include "vccc/functional/invoke.hpp"
#include "vccc/iterator/indirectly_readable.hpp"
#include "vccc/iterator/iter_common_reference_t.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"

namespace vccc {
namespace detail {

template<
    typename F,
    typename I,
    bool v = conjunction<
                 invocable<F&, iter_value_t<I>&>,
                 invocable<F&, iter_reference_t<I>>,
                 invocable<F&, iter_common_reference_t<I>>
             >::value /* true */
>
struct indirectly_unary_invocable_impl_2
    : common_reference_with<
          invoke_result_t<F&, iter_value_t<I>&>,
          invoke_result_t<F&, iter_reference_t<I>>
      > {};

template<typename F, typename I>
struct indirectly_unary_invocable_impl_2<F, I, false> : std::false_type {};

template<
    typename F,
    typename I,
    bool v = conjunction<
                 indirectly_readable<I>,
                 copy_constructible<F>,
                 has_typename_type< iter_common_reference<I> >
             >::value /* true */
>
struct indirectly_unary_invocable_impl_1 : indirectly_unary_invocable_impl_2<F, I> {};

template<typename F, typename I>
struct indirectly_unary_invocable_impl_1<F, I, false> : std::false_type {};


} // namespace detail

/// @addtogroup iterator
/// @{

/**
@brief specifies that a callable type can be invoked with the result of dereferencing an `indirectly_readable` type

The concepts `%indirectly_unary_invocable` and `%indirectly_regular_unary_invocable` specify requirements for algorithms
that call (regular) unary invocables as their arguments. The key difference between these concepts and `vccc::invocable`
is that they are applied to the type the `I` references, rather than `I` itself.

@sa [std::indirectly_unary_invocable, std::indirectly_regular_unary_invocable](https://en.cppreference.com/w/cpp/iterator/indirectly_unary_invocable)
 */

template<typename F, typename I>
struct indirectly_unary_invocable : detail::indirectly_unary_invocable_impl_1<F, I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_INDIRECT_UNARY_INVOCABLE_HPP
