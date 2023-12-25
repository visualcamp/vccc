//
// Created by cosge on 2023-12-03.
//

#ifndef VCCC_ITERATOR_BIDIRECTIONAL_ITERATOR_HPP_
#define VCCC_ITERATOR_BIDIRECTIONAL_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/derived_from.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/iterator/iterator_traits/legacy_bidirectional_iterator.hpp"
#include "vccc/iterator/forward_iterator.hpp"
#include "vccc/iterator/iterator_tag.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct is_post_decrementable_bidi_iter : std::false_type {};

template<typename T>
struct is_post_decrementable_bidi_iter<T, void_t<decltype( std::declval<T&>()-- )>>
    : same_as<decltype( std::declval<T&>()-- ), T> {};

template<typename I, bool = forward_iterator<I>::value>
struct bidirectional_iterator_impl : std::false_type {};

template<typename I>
struct bidirectional_iterator_impl<I, true>
    : conjunction<
        derived_from<ITER_CONCEPT<I>, bidirectional_iterator_tag>,
        is_pre_decrementable<I>,
        is_post_decrementable_bidi_iter<I>
      > {};

} // namespace detail

/// @addtogroup iterator
/// @{


/**
@brief specifies that a `forward_iterator` is a bidirectional iterator, supporting movement backwards

The concept `%bidirectional_iterator` refines `forward_iterator` by adding the ability to move an iterator backward.

<H1>Notes</H1>
Unlike the `LegacyBidirectionalIterator` requirements, the `%bidirectional_iterator` concept does not require
dereference to return an lvalue.

@sa [std::bidirectional_iterator](https://en.cppreference.com/w/cpp/iterator/bidirectional_iterator)
@sa forward_iterator
@sa random_access_iterator

*/
template<typename I>
struct bidirectional_iterator : detail::bidirectional_iterator_impl<I> {};

/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_BIDIRECTIONAL_ITERATOR_HPP_
