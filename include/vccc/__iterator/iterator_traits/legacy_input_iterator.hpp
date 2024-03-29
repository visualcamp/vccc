//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_INPUT_ITERATOR_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_INPUT_ITERATOR_HPP_

#include <type_traits>

#include "vccc/__concepts/equality_comparable.hpp"
#include "vccc/__concepts/signed_integral.hpp"
#include "vccc/__iterator/iterator_traits/legacy_iterator.hpp"
#include "vccc/__iterator/incrementable_traits.hpp"
#include "vccc/__iterator/indirectly_readable_traits.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__type_traits/common_reference.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/has_typename_value_type.hpp"
#include "vccc/__type_traits/has_typename_difference_type.hpp"

namespace vccc {
namespace detail {

template<
    typename I,
    bool = conjunction<
             LegacyIterator<I>,
             equality_comparable<I>,
             has_typename_difference_type<incrementable_traits<I>>,
             has_typename_value_type<indirectly_readable_traits<I>>,
             has_typename_type<iter_reference<I>>
           >::value
>
struct LegacyInputIteratorRequiresStage1 : std::false_type {};

template<typename I>
struct LegacyInputIteratorRequiresStage1<I, true>
    : conjunction<
        has_typename_type<common_reference<iter_reference_t<I>&&,
                                           typename indirectly_readable_traits<I>::value_type&>>,
        has_typename_type<common_reference<decltype(*std::declval<I&>()++)&&, // *i++ satisfied in LegacyIterator<I>
                                           typename indirectly_readable_traits<I>::value_type&>>,
        signed_integral<typename incrementable_traits<I>::difference_type>
      > {};

} // namespace detail

/**
@addtogroup iterator
@{
@addtogroup iterator_LegacyInputIterator__Named_Requirements LegacyInputIterator
@{
 */

template<typename I, bool = is_referencable<I>::value>
struct LegacyInputIterator : std::false_type {};

template<typename I>
struct LegacyInputIterator<I, true> : detail::LegacyInputIteratorRequiresStage1<I> {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_INPUT_ITERATOR_HPP_
