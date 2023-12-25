//
// Created by yonggyulee on 2023/12/21.
//

#ifndef VCCC_CONCEPTS_IMPLICIT_EXPRESSION_CHECK_HPP_
#define VCCC_CONCEPTS_IMPLICIT_EXPRESSION_CHECK_HPP_

#include "vccc/type_traits/conjunction.hpp"

namespace vccc {
namespace detail {

template<template<typename, typename, typename...> class Check, typename Left, typename Right>
struct implicit_expression_check_impl
    : conjunction<
        Check<Left, Right&>,
        Check<Left, Right&&>,
        Check<Left, const Right&>,
        Check<Left, const Right&&>
      > {};

} // namespace detail

/**
 *
 * Implicit expression variations
 *
 * A requires expression that uses an expression that is non-modifying for some
 * constant lvalue operand also implicitly requires additional variations of that
 * expression that accept a non-constant lvalue or (possibly constant) rvalue for
 * the given operand unless such an expression variation is explicitly required with
 * differing semantics.
 *
 * These implicit expression variations must meet the same semantic requirements of
 * the declared expression. The extent to which an implementation validates the syntax
 * of the variations is unspecified.
*/

template<template<typename, typename...> class Check, typename Operand, typename...>
struct implicit_expression_check;

template<template<typename, typename, typename...> class Check, typename Left, typename Right>
struct implicit_expression_check<Check, Left, Right> : Check<Left, Right> {};

template<template<typename, typename, typename...> class Check, typename Left, typename Right>
struct implicit_expression_check<Check, Left, const Right&>
    : detail::implicit_expression_check_impl<Check, Left, Right> {};

template<template<typename, typename, typename...> class Check, typename Left, typename Right>
struct implicit_expression_check<Check, const Left&, const Right&>
    : conjunction<
        detail::implicit_expression_check_impl<Check, Left&, Right>,
        detail::implicit_expression_check_impl<Check, Left&&, Right>,
        detail::implicit_expression_check_impl<Check, const Left&, Right>,
        detail::implicit_expression_check_impl<Check, const Left&&, Right>
      > {};

template<template<typename, typename...> class Check, typename Operand>
struct implicit_expression_check<Check, Operand> : Check<Operand> {};

template<template<typename, typename...> class Check, typename Operand>
struct implicit_expression_check<Check, const Operand&> : conjunction<
        Check<Operand&>,
        Check<Operand&&>,
        Check<const Operand&>,
        Check<const Operand&&>
      > {};

} // namespace vccc

#endif // VCCC_CONCEPTS_IMPLICIT_EXPRESSION_CHECK_HPP_
