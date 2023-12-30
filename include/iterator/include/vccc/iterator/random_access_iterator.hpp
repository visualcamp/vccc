//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_RANDOM_ACCESS_ITERATOR_HPP_
#define VCCC_ITERATOR_RANDOM_ACCESS_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/derived_from.hpp"
#include "vccc/concepts/implicit_expression_check.hpp"
#include "vccc/concepts/totally_ordered.hpp"
#include "vccc/iterator/detail/iter_concept.hpp"
#include "vccc/iterator/bidirectional_iterator.hpp"
#include "vccc/iterator/iterator_tag.hpp"
#include "vccc/iterator/iter_difference_t.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/sized_sentinel_for.hpp"
#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/is_referenceable.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4244)
#endif

namespace vccc {
namespace detail {
namespace detail_random_access_iterator {

template<typename I, typename D, typename = void, typename = void>
struct explicit_op_assign_check : std::false_type {};
template<typename I, typename D>
struct explicit_op_assign_check<
      I, D,
      void_t<decltype( std::declval<I>() += std::declval<D>() )>,
      void_t<decltype( std::declval<I>() -= std::declval<D>() )>
    >
    : conjunction<
        same_as<decltype( std::declval<I>() += std::declval<D>() ), std::add_lvalue_reference_t<std::remove_reference_t<I>>>,
        same_as<decltype( std::declval<I>() -= std::declval<D>() ), std::add_lvalue_reference_t<std::remove_reference_t<I>>>
      > {};

template<typename I, typename D, typename = void, typename = void, typename = void, typename = void>
struct explicit_op_const_check : std::false_type {};
template<typename I, typename D>
struct explicit_op_const_check<
      I, D,
      void_t<decltype( std::declval<I>() + std::declval<D>() )>,
      void_t<decltype( std::declval<D>() + std::declval<I>() )>,
      void_t<decltype( std::declval<I>() - std::declval<D>() )>,
      void_t<decltype( std::declval<I>()[std::declval<D>()] )>
    >
    : conjunction<
        same_as<decltype( std::declval<I>() + std::declval<D>() ), remove_cvref_t<I>>,
        same_as<decltype( std::declval<D>() + std::declval<I>() ), remove_cvref_t<I>>,
        same_as<decltype( std::declval<I>() - std::declval<D>() ), remove_cvref_t<I>>,
        same_as<decltype( std::declval<I>()[std::declval<D>()] ), iter_reference_t<remove_cvref_t<I>>>
      > {};

template<typename I, bool = is_referencable<iter_difference_t<I>>::value /* false */>
struct op_check : std::false_type {};

template<typename I>
struct op_check<I, true>
    : conjunction<
        implicit_expression_check<explicit_op_assign_check, I&, const iter_difference_t<I>&>,
        implicit_expression_check<explicit_op_const_check, const I&, const iter_difference_t<I>&>
      > {};

} // detail_random_access_iterator

template<
    typename I,
    bool =
        conjunction<
          bidirectional_iterator<I>,
          has_typename_type<iter_difference<I>>,
          has_typename_type<iter_reference<I>>
        >::value /* false */
>
struct random_access_iterator_impl : std::false_type {};

template<typename I>
struct random_access_iterator_impl<I, true>
    : conjunction<
        derived_from<ITER_CONCEPT<I>, random_access_iterator_tag>,
        totally_ordered<I>,
        sized_sentinel_for<I, I>,
        detail_random_access_iterator::op_check<I>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{


/**
@brief specifies that a `bidirectional_iterator` is a random-access iterator, supporting advancement in constant time
and subscripting

The concept `%random_access_iterator` refines `bidirectional_iterator` by adding support for constant time advancement
with the `+=`, `+`, `-=`, and `-` operators, constant time computation of distance with `-`, and array notation with
subscripting `[]`.



<H1>Notes</H1>
Unlike the `LegacyRandomAccessIterator` requirements, the `%random_access_iterator` concept does not require dereference
to return an lvalue.

@sa [std::random_access_iterator](https://en.cppreference.com/w/cpp/iterator/random_access_iterator)
@sa bidirectional_iterator
@sa contiguous_iterator
*/

template<typename I>
struct random_access_iterator : detail::random_access_iterator_impl<I> {};

/// @}

} // namespace vccc

#ifdef _MSC_VER
#pragma warning( pop )
#endif

#endif // VCCC_ITERATOR_RANDOM_ACCESS_ITERATOR_HPP_
