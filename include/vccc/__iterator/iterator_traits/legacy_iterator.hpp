//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_ITERATOR_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_ITERATOR_HPP_

#include <type_traits>

#include "vccc/__concepts/copyable.hpp"
#include "vccc/__concepts/dereferenceable.hpp"
#include "vccc/__iterator/weakly_incrementable.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/is_referenceable.hpp"

namespace vccc {
namespace detail {

template<typename I, bool = dereferenceable<decltype(std::declval<I&>()++)>::value>
struct LegacyIteratorCheckPostIncrement_2 : std::false_type {};
template<typename I>
struct LegacyIteratorCheckPostIncrement_2<I, true> : is_referencable<decltype(*std::declval<I&>()++)> {};

template<typename I, bool = vccc::detail::is_post_incrementable<I>::value>
struct LegacyIteratorCheckPostIncrement : std::false_type {};
template<typename I>
struct LegacyIteratorCheckPostIncrement<I, true> : LegacyIteratorCheckPostIncrement_2<I> {};

} // namespace detail

/**
@addtogroup iterator
@{
@addtogroup iterator_LegacyIterator__Named_Requirements LegacyIterator
@{
 */

template<typename I, bool = dereferenceable<I>::value>
struct LegacyIterator : std::false_type {};

template<typename I>
struct LegacyIterator<I, true>
    : conjunction<
        is_referencable<decltype(*std::declval<I&>())>,
        detail::is_pre_incrementable<I>,
        detail::LegacyIteratorCheckPostIncrement<I>,
        copyable<I>
> {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_ITERATOR_HPP_
