//
// Created by yonggyulee on 2023/12/27.
//

#ifndef VCCC_CONCEPTS_COMPARISON_COMMON_TYPE_WITH_HPP
#define VCCC_CONCEPTS_COMPARISON_COMMON_TYPE_WITH_HPP

#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__concepts/same_as.hpp"
#include "vccc/__concepts/weakly_equality_comparable_with.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_type.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename T, typename U, typename C = common_reference_t<const T&, const U&>>
struct comparison_common_type_with_impl_3
    : conjunction<
        same_as<common_reference_t<const T&, const U&>,
                common_reference_t<const U&, const T&>>,
        disjunction<convertible_to<const T&, const C&>,
                    convertible_to<T, const C&>>,
        disjunction<convertible_to<const U&, const C&>,
                    convertible_to<U, const C&>>
      > {};

template<
    typename T,
    typename U,
    bool = conjunction<has_typename_type< common_reference<const T&, const U&> >,
                       has_typename_type< common_reference<const U&, const T&> >>::value /* true */
>
struct comparison_common_type_with_impl_2 : comparison_common_type_with_impl_3<T, U> {};

template<typename T, typename U>
struct comparison_common_type_with_impl_2<T, U, false> : std::false_type {};

template<typename T, typename U, bool = conjunction<is_referencable<T>, is_referencable<U>>::value /* true */>
struct comparison_common_type_with_impl_1 : comparison_common_type_with_impl_2<T, U> {};

template<typename T, typename U>
struct comparison_common_type_with_impl_1<T, U, false> : std::false_type {};

} // namespace detail

/// @addtogroup concepts
/// @{

/**
@brief specifies that two types share a common type, and a const lvalue or a non-const rvalue of either type is
convertible to that common type.

`%comparison_common_type_with` \copybrief comparison_common_type_with

<H1>Notes</H1>
`%vccc::comparison_common_type_with` is implemented in C++23 version

@sa [__ComparisonCommonTypeWith](https://en.cppreference.com/w/cpp/concepts/equality_comparable)
 */
template<typename T, typename U>
struct comparison_common_type_with
  : detail::comparison_common_type_with_impl_1<remove_cvref_t<T>, remove_cvref_t<U>> {};

/// @}

} // namespace vccc

#endif // VCCC_CONCEPTS_COMPARISON_COMMON_TYPE_WITH_HPP
