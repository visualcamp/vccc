//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_PREDICATE_HPP
#define VCCC_CONCEPTS_PREDICATE_HPP

#include "vccc/concepts/boolean_testable.hpp"
#include "vccc/concepts/invocable.hpp"
#include "vccc/functional/invoke.hpp"

namespace vccc {
namespace detail {

template<bool v /* false */, typename F, typename... Args>
struct predicate_impl : std::false_type {};

template<typename F, typename... Args>
struct predicate_impl<true, F, Args...> : boolean_testable<invoke_result_t<F, Args...>> {};

} // namespace detail
/// @addtogroup concepts
/// @{

/**
@brief specifies that a callable type is a Boolean predicate

The concept `%vccc::predicate<F, Args...>` specifies that `F` is a predicate that accepts arguments whose types and
value categories are encoded by `Args...`, i.e., it can be invoked with these arguments to produce a
\ref concepts_boolean_testable__class__Comparison_concepts "boolean-testable" result.

Note that regular_invocable requires the invocation to not modify either the callable object or the arguments and be equality-preserving.


@sa [std::predicate](https://en.cppreference.com/w/cpp/concepts/predicate)
 */
template<typename F, typename... Args>
struct predicate : detail::predicate_impl<regular_invocable<F, Args...>::value, F, Args...> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_PREDICATE_HPP
