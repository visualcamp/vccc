//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CONCEPTS_BOOLEAN_TESTABLE_HPP_
#define VCCC_CONCEPTS_BOOLEAN_TESTABLE_HPP_

#include <type_traits>

#include "vccc/concepts/convertible_to.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace detail {

template<typename B>
struct boolean_testable_impl : convertible_to<B, bool> {};

template<typename T, typename = void>
struct is_explicitly_negatable : std::false_type {};
template<typename T>
struct is_explicitly_negatable<T, void_t<decltype(!std::declval<T>())> > : std::true_type {};

template<
    typename B,
    bool =
        conjunction<
            is_explicitly_negatable<std::add_lvalue_reference_t<std::remove_reference_t<B>>>,
            is_explicitly_negatable<std::add_rvalue_reference_t<std::remove_reference_t<B>>>
        >::value
>
struct boolean_testable_stage_2 : std::false_type {};

template<typename B>
struct boolean_testable_stage_2<B, true>
    : conjunction<
        boolean_testable_impl<decltype(!std::declval< std::add_lvalue_reference_t<std::remove_reference_t<B>> >())>,
        boolean_testable_impl<decltype(!std::declval< std::add_rvalue_reference_t<std::remove_reference_t<B>> >())>
      > {};

template<
    typename B,
    bool = conjunction<boolean_testable_impl<B>, is_referencable<B>>::value
>
struct boolean_testable_stage_1 : std::false_type {};

template<typename B>
struct boolean_testable_stage_1<B, true> : boolean_testable_stage_2<B> {};

} // namespace detail

/// @addtogroup concepts
/// @{
/// @addtogroup concepts_boolean_testable__class__Comparison_concepts boolean_testable
/// @brief specifies that a type can be used in Boolean contexts
/// @{

/**

The exposition-only concept \a boolean-testable specifies the requirements for expressions that are convertible to
`bool` and for which the logical operators have the usual behavior (including short-circuiting), even for two different
\a boolean-testable types.

@sa [std::boolean-testable](https://en.cppreference.com/w/cpp/concepts/boolean-testable)
 */

template<typename B>
struct boolean_testable : detail::boolean_testable_stage_1<B> {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_BOOLEAN_TESTABLE_HPP_
