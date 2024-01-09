//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CONCEPTS_PARTIALLY_ORDERED_WITH_HPP_
#define VCCC_CONCEPTS_PARTIALLY_ORDERED_WITH_HPP_

#include <type_traits>

#include "vccc/__concepts/implicit_expression_check.hpp"
#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace detail {

template<typename T, typename U>
struct explicit_less_than_comparable : bool_constant<rel_ops::is_less_than_comparable<T, U>()> {};

template<typename T, typename U>
struct explicit_less_equal_than_comparable : bool_constant<rel_ops::is_less_equal_than_comparable<T, U>()> {};

template<typename T, typename U>
struct explicit_greater_than_comparable : bool_constant<rel_ops::is_greater_than_comparable<T, U>()> {};

template<typename T, typename U>
struct explicit_greater_equal_than_comparable : bool_constant<rel_ops::is_greater_equal_than_comparable<T, U>()> {};

template<typename T, typename U, bool = conjunction<is_referencable<T>, is_referencable<U>>::value>
struct partially_ordered_with_impl : std::false_type {};
template<typename T, typename U>
struct partially_ordered_with_impl<T, U, true>
    : conjunction<
        implicit_expression_check<explicit_less_than_comparable,             const std::remove_reference_t<T>&, const std::remove_reference_t<U>& >,
        implicit_expression_check<explicit_less_equal_than_comparable,    const std::remove_reference_t<T>&, const std::remove_reference_t<U>& >,
        implicit_expression_check<explicit_greater_than_comparable,          const std::remove_reference_t<T>&, const std::remove_reference_t<U>& >,
        implicit_expression_check<explicit_greater_equal_than_comparable, const std::remove_reference_t<T>&, const std::remove_reference_t<U>& >,
        implicit_expression_check<explicit_less_than_comparable,             const std::remove_reference_t<U>&, const std::remove_reference_t<T>& >,
        implicit_expression_check<explicit_less_equal_than_comparable,    const std::remove_reference_t<U>&, const std::remove_reference_t<T>& >,
        implicit_expression_check<explicit_greater_than_comparable,          const std::remove_reference_t<U>&, const std::remove_reference_t<T>& >,
        implicit_expression_check<explicit_greater_equal_than_comparable, const std::remove_reference_t<U>&, const std::remove_reference_t<T>& >
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
 *  The `partially_ordered_with` specifies that a value of type `T` and a value of type `U` can be compared
 *  in a partial order with each other (in either order) using `<`, `>`, `<=`, and `>=`,
 *  and the results of the comparisons are consistent.
 */
template<typename T, typename U>
struct partially_ordered_with : detail::partially_ordered_with_impl<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_PARTIALLY_ORDERED_WITH_HPP_
