//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_RANDOM_ACCESS_ITERATOR_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_RANDOM_ACCESS_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/convertible_to.hpp"
#include "vccc/concepts/same_as.hpp"
#include "vccc/concepts/totally_ordered.hpp"
#include "vccc/iterator/iterator_traits/legacy_bidirectional_iterator.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/void_t.hpp"

namespace vccc {
namespace detail {

template<typename I, typename D, typename = void>
struct is_iter_add_assignmentable : std::false_type {};
template<typename I, typename D>
struct is_iter_add_assignmentable<I, D, void_t<decltype(std::declval<I&>() += std::declval<D&>())>>
    : concepts::same_as<decltype(std::declval<I&>() += std::declval<D&>()), I&> {};

template<typename I, typename D, typename = void>
struct is_iter_sub_assignmentable : std::false_type {};
template<typename I, typename D>
struct is_iter_sub_assignmentable<I, D, void_t<decltype(std::declval<I&>() -= std::declval<D&>())>>
    : concepts::same_as<decltype(std::declval<I&>() -= std::declval<D&>()), I&> {};

template<typename I, typename D, typename = void, typename = void>
struct is_iter_addable : std::false_type {};
template<typename I, typename D>
struct is_iter_addable<I, D,
                       void_t<decltype(std::declval<I&>() + std::declval<D&>())>,
                       void_t<decltype(std::declval<D&>() + std::declval<I&>())>>
    : conjunction<
        concepts::same_as<decltype(std::declval<I&>() + std::declval<D&>()), I>,
        concepts::same_as<decltype(std::declval<D&>() + std::declval<I&>()), I>
      >{};

template<typename I, typename D, typename = void, typename = void>
struct is_iter_subtractable : std::false_type {};
template<typename I, typename D>
struct is_iter_subtractable<I, D,
                            void_t<decltype(std::declval<I&>() - std::declval<D&>())>,
                            void_t<decltype(std::declval<I&>() - std::declval<I&>())>>
    : conjunction<
        concepts::same_as<decltype(std::declval<I&>() - std::declval<D&>()), I>,
        concepts::same_as<decltype(std::declval<I&>() - std::declval<I&>()), D>
      > {};

template<typename I, typename D, typename = void>
struct is_iter_subscriptable : std::false_type {};

template<typename I, typename D>
struct is_iter_subscriptable<I, D, void_t<decltype( std::declval<I&>()[std::declval<D&>()] )>>
    : concepts::convertible_to<decltype( std::declval<I&>()[std::declval<D&>()] ), iter_reference_t<I>> {};

template<typename I, typename D>
struct LegacyRandomAccessIteratorStage2
    : conjunction<
        is_iter_add_assignmentable<I, D>,
        is_iter_sub_assignmentable<I, D>,
        is_iter_addable<I, D>,
        is_iter_subtractable<I, D>,
        is_iter_subscriptable<I, D>
      > {};

template<typename I, typename D, bool = is_referencable<D>::value>
struct LegacyRandomAccessIteratorStage1 : std::false_type {};

template<typename I, typename D>
struct LegacyRandomAccessIteratorStage1<I, D, true> : LegacyRandomAccessIteratorStage2<I, D> {};

} // namespace detail

template<typename I, bool = conjunction<LegacyBidirectionalIterator<I>, concepts::totally_ordered<I>>::value>
struct LegacyRandomAccessIterator : std::false_type {};

template<typename I>
struct LegacyRandomAccessIterator<I, true>
    : detail::LegacyRandomAccessIteratorStage1<I, typename incrementable_traits<I>::difference_type> {};

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_RANDOM_ACCESS_ITERATOR_HPP_