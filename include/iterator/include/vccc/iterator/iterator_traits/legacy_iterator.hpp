//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_ITERATOR_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_ITERATOR_HPP_

#include <type_traits>

#include "vccc/concepts/copyable.hpp"
#include "vccc/concepts/dereferenceable.hpp"
#include "vccc/iterator/weakly_incrementable.hpp"
#include "vccc/type_traits/conjunction.hpp"
#include "vccc/type_traits/is_referenceable.hpp"

namespace vccc {
namespace detail {

template<typename I, bool = concepts::dereferenceable<decltype(std::declval<I&>()++)>::value>
struct LegacyIteratorCheckPostIncrement_2 : std::false_type {};
template<typename I>
struct LegacyIteratorCheckPostIncrement_2<I, true> : is_referencable<decltype(*std::declval<I&>()++)> {};

template<typename I, bool = concepts::detail::is_post_incrementable<I>::value>
struct LegacyIteratorCheckPostIncrement : std::false_type {};
template<typename I>
struct LegacyIteratorCheckPostIncrement<I, true> : LegacyIteratorCheckPostIncrement_2<I> {};

} // namespace detail

template<typename I, bool = concepts::dereferenceable<I>::value>
struct LegacyIterator : std::false_type {};

template<typename I>
struct LegacyIterator<I, true>
    : conjunction<
        is_referencable<decltype(*std::declval<I&>())>,
        concepts::detail::is_post_incrementable<I>,
        detail::LegacyIteratorCheckPostIncrement<I>,
        concepts::copyable<I>
      > {};

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_ITERATOR_HPP_
