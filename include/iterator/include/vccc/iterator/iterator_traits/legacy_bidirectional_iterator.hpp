//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_BIDIRECTIONAL_ITERATOR_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_BIDIRECTIONAL_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/constructible_from.hpp"
#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/iterator/iterator_traits/legacy_forward_iterator.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/iterator/iter_reference_t.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<typename T, typename = void>
struct is_pre_decrementable : std::false_type {};

template<typename T>
struct is_pre_decrementable<T, void_t<decltype( --std::declval<T&>() )>>
    : concepts::same_as<decltype( --std::declval<T&>() ), T&> {};

template<typename T, typename = void>
struct is_post_decrementable : std::false_type {};

template<typename T>
struct is_post_decrementable<T, void_t<decltype( std::declval<T&>()-- )>>
      : concepts::convertible_to<decltype(std::declval<T&>()--), const T&> {};

template<
    typename It,
    bool = concepts::dereferenceable<decltype(std::declval<It&>()--)>::value
>
struct LegacyBidirectionalIteratorRequireStage2 : std::false_type {};

template<typename It>
struct LegacyBidirectionalIteratorRequireStage2<It, true> // vvvvvvvvvvvvv already satisfied
    : concepts::same_as<decltype(*std::declval<It&>()--), iter_reference_t<It>> {};
// iter_reference_t<It> already satisfied in LegacyInputIterator ^^^^^^^^^

template<
    typename It,
    bool =
        conjunction<
          is_pre_decrementable<It>,
          is_post_decrementable<It>
        >::value
>
struct LegacyBidirectionalIteratorRequireStage1 : std::false_type {};

template<typename It>
struct LegacyBidirectionalIteratorRequireStage1<It, true> : LegacyBidirectionalIteratorRequireStage2<It> {};

} // namespace detail

template<typename It, bool = LegacyForwardIterator<It>::value>
struct LegacyBidirectionalIterator : std::false_type {};

template<typename It>
struct LegacyBidirectionalIterator<It, true> : detail::LegacyBidirectionalIteratorRequireStage1<It> {};

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_BIDIRECTIONAL_ITERATOR_HPP_
