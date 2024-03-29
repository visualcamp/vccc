//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_WEAKLY_EQUALITY_COMPARABLE_WITH_HPP_
#define VCCC_CONCEPTS_WEAKLY_EQUALITY_COMPARABLE_WITH_HPP_

#include <type_traits>

#include "vccc/__type_traits/bool_constant.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__utility/cxx20_rel_ops.hpp"

namespace vccc {
namespace detail {

template<
    typename T,
    typename U,
    bool = conjunction<
             is_referencable<std::remove_reference_t<T>>,
             is_referencable<std::remove_reference_t<U>>
           >::value
>
struct weakly_equality_comparable_with_impl : std::false_type {};

template<typename T, typename U>
struct weakly_equality_comparable_with_impl<T, U, true>
    : conjunction<
        rel_ops::is_equality_comparable<const std::remove_reference_t<T>&, const std::remove_reference_t<U>& >,
        rel_ops::is_equality_comparable<const std::remove_reference_t<U>&, const std::remove_reference_t<T>& >,
        rel_ops::is_non_equality_comparable<const std::remove_reference_t<T>&, const std::remove_reference_t<U>& >,
        rel_ops::is_non_equality_comparable<const std::remove_reference_t<U>&, const std::remove_reference_t<T>& >
      > {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that two different objects can be compared for equality with each other (in either order) using both `==` and `!=`

`%weakly_equality_comparable_with` specifies that an object of type `T` and an object of type `U` can be compared for
equality with each other (in either order) using both `==` and `!=`, and the results of the comparisons are consistent.

@sa [__WeaklyEqualityComparableWith](https://en.cppreference.com/w/cpp/concepts/equality_comparable)
 */
template<typename T, typename U>
struct weakly_equality_comparable_with : detail::weakly_equality_comparable_with_impl<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_WEAKLY_EQUALITY_COMPARABLE_WITH_HPP_
