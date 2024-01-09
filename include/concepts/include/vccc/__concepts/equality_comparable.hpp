//
// Created by yonggyulee on 2023/12/24.
//

#ifndef VCCC_CONCEPTS_EQUALITY_COMPARABLE_HPP_
#define VCCC_CONCEPTS_EQUALITY_COMPARABLE_HPP_

#include "vccc/__concepts/comparison_common_type_with.hpp"
#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__concepts/weakly_equality_comparable_with.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{

/**
@brief specifies that operator `==` is an equivalence relation

The concept `vccc::equality_comparable` specifies that the comparison operators `==` and `!=` on `T`
reflects equality: `==` yields `true` if and only if the operands are equal.


@sa [std::equality_comparable](https://en.cppreference.com/w/cpp/concepts/equality_comparable)
 */
template<typename T>
struct equality_comparable : weakly_equality_comparable_with<T, T> {};


namespace detail {

template<
    typename T,
    typename U,
    bool = has_typename_type<
             common_reference< const std::remove_reference_t<T>&,
                               const std::remove_reference_t<U>& >
           >::value /* true */
>
struct equality_comparable_with_impl_2
    : conjunction<
        equality_comparable<
            common_reference_t< const std::remove_reference_t<T>&,
                                const std::remove_reference_t<U>&> >,
        weakly_equality_comparable_with<T, U>
      >{};

template<typename T, typename U>
struct equality_comparable_with_impl_2<T, U, false> : std::false_type {};

template<
    typename T,
    typename U,
    bool = conjunction<
               equality_comparable<T>,
               equality_comparable<U>,
               comparison_common_type_with<T, U>
           >::value /* true */
>
struct equality_comparable_with_impl_1 : equality_comparable_with_impl_2<T, U> {};
template<typename T, typename U>
struct equality_comparable_with_impl_1<T, U, false> : std::false_type {};

} // namespace detail

/**
\brief \copybrief equality_comparable

The concept `%vccc::equality_comparable_with` specifies that the comparison operators `==` and `!=` on (possibly mixed)
`T` and `U` operands yield results consistent with equality. Comparing mixed operands yields results equivalent to
comparing the operands converted to their common type.

@sa [std::equality_comparable_with](https://en.cppreference.com/w/cpp/concepts/equality_comparable)
 */
template<typename T, typename U>
struct equality_comparable_with : detail::equality_comparable_with_impl_1<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_EQUALITY_COMPARABLE_HPP_
