//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_CONTIGUOUS_ITERATOR_HPP_
#define VCCC_ITERATOR_CONTIGUOUS_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/derived_from.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/iterator/detail/iter_concept.hpp"
#include "vccc/iterator/iterator_traits/legacy_random_access_iterator.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_value_t.hpp"
#include "vccc/iterator/iterator_tag.hpp"
#include "vccc/iterator/random_access_iterator.hpp"
#include "vccc/memory/to_address.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/has_typename_type.hpp"
#include "vccc/type_traits/remove_cvref.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename I, typename = void>
struct explicit_contiguous_requires : std::false_type {};
template<typename I>
struct explicit_contiguous_requires<
        I,
        void_t<decltype( vccc::to_address( std::declval<I>() ) )>
    > : same_as<decltype( vccc::to_address( std::declval<I>() ) ), std::add_pointer_t<iter_reference_t<remove_cvref_t<I>>>> {};

template<typename I>
struct contiguous_requires : implicit_expression_check<explicit_contiguous_requires, const I&> {};

template<
    typename I,
    bool =
        conjunction<
          random_access_iterator<I>,
          has_typename_type<iter_value<I>>
        >::value
>
struct contiguous_iterator_impl : std::false_type {};

template<typename I>
struct contiguous_iterator_impl<I, true>
    : conjunction<
#if __cplusplus < 202002L
        disjunction<
#endif
            derived_from<ITER_CONCEPT<I>, contiguous_iterator_tag>,
#if __cplusplus < 202002L
            conjunction<
              derived_from<ITER_CONCEPT<I>, random_access_iterator_tag>,
              LegacyRandomAccessIterator<I>
            >
        >,
#endif
        std::is_lvalue_reference<iter_reference_t<I>>,
        same_as<iter_value_t<I>, remove_cvref_t<iter_reference_t<I>>>,
        contiguous_requires<I>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{




/**
@brief specifies that a `random_access_iterator` is a contiguous iterator, referring to elements that are contiguous in memory

The `%contiguous_iterator` concept refines `random_access_iterator` by providing a guarantee the denoted elements are
stored contiguously in the memory.

<H1>Notes</H1>
`%contiguous_iterator` is modeled by every pointer type to complete object type.

Iterator types in the standard library that are required to satisfy the `LegacyContiguousIterator` requirements in C++17
are also required to model `%contiguous_iterator` in C++20.

@sa [std::contiguous_iterator](https://en.cppreference.com/w/cpp/iterator/contiguous_iterator)
@sa random_access_iterator
*/

template<typename I>
struct contiguous_iterator : detail::contiguous_iterator_impl<I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_CONTIGUOUS_ITERATOR_HPP_
