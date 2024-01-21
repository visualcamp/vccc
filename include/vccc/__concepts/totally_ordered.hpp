//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_CONCEPTS_TOTALLY_ORDERED_HPP_
#define VCCC_CONCEPTS_TOTALLY_ORDERED_HPP_

#include <type_traits>

#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__concepts/partially_ordered_with.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/conjunction.hpp"

namespace vccc {

/// @addtogroup concepts
/// @{


/**
@brief specifies that the comparison operators on the type yield a total order

The concept `totally_ordered` specifies that the comparison operators `==`,`!=`,`<`,`>`,`<=`,`>=` on a type yield
results consistent with a [strict total order](https://en.wikipedia.org/wiki/Total_order#Strict_and_non-strict_total_orders)
on the type.

@sa [std::totally_ordered](https://en.cppreference.com/w/cpp/concepts/totally_ordered)
@sa totally_ordered_with
 */
template<typename T>
struct totally_ordered
    : conjunction<
        equality_comparable<T>,
        partially_ordered_with<T, T>
      >{};


namespace detail {

template<
    typename T,
    typename U,
    bool = conjunction<
               totally_ordered<T>,
               totally_ordered<U>,
               equality_comparable_with<T, U>
           >::value /* false */
>
struct totally_ordered_with_impl : std::false_type {};

template<typename T, typename U>
struct totally_ordered_with_impl<T, U, true>
    : conjunction<
        totally_ordered<
            common_reference_t< const std::remove_reference_t<T>&,
                                const std::remove_reference_t<U>&> >,
        partially_ordered_with<T, U>
      > {};

} // namespace detail

/**
\brief \copybrief totally_ordered

The concept `%vccc::totally_ordered_with` specifies that the comparison operators `==`,`!=`,`<`,`>`,`<=`,`>=` on
(possibly mixed) `T` and `U` operands yield results consistent with a strict total order.
Comparing mixed operands yields results equivalent to comparing the operands converted to their common type.

@sa [std::totally_ordered_with](https://en.cppreference.com/w/cpp/concepts/totally_ordered)
@sa totally_ordered
 */
template<typename T, typename U>
struct totally_ordered_with : detail::totally_ordered_with_impl<T, U> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_TOTALLY_ORDERED_HPP_
