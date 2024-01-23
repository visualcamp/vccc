//
// Created by yonggyulee on 2023/12/25.
//

#ifndef VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_FORWARD_ITERATOR_HPP_
#define VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_FORWARD_ITERATOR_HPP_

#include <type_traits>

#include "vccc/__concepts/constructible_from.hpp"
#include "vccc/__concepts/convertible_to.hpp"
#include "vccc/__iterator/iterator_traits/legacy_input_iterator.hpp"
#include "vccc/__iterator/indirectly_readable_traits.hpp"
#include "vccc/__iterator/iter_reference_t.hpp"
#include "vccc/__type_traits/conjunction.hpp"
#include "vccc/__type_traits/remove_cvref.hpp"

namespace vccc {
namespace detail {

template<
    typename It,
    bool =
        conjunction<
          constructible_from<It>,
          std::is_reference<iter_reference_t<It>>, // iter_reference_t<It> satisfied in LegacyInputIterator,
          same_as<
              remove_cvref_t<iter_reference_t<It>>,
              typename indirectly_readable_traits<It>::value_type>, // indirectly_readable_traits<It>::value already satisfied
          convertible_to<decltype(std::declval<It&>()++),  const It&>,
          same_as<decltype(*std::declval<It&>()++), iter_reference_t<It>>
        >::value
>
struct LegacyForwardIteratorRequire : std::false_type {};

template<typename It>
struct LegacyForwardIteratorRequire<It, true> : std::true_type {};

} // namespace detail

/**
@addtogroup iterator
@{
@addtogroup iterator_LegacyForwardIterator__Named_Requirements LegacyForwardIterator
@{
 */

template<typename It, bool = LegacyInputIterator<It>::value>
struct LegacyForwardIterator : std::false_type {};

template<typename It>
struct LegacyForwardIterator<It, true> : detail::LegacyForwardIteratorRequire<It> {};

/// @}
/// @}

} // namespace vccc

#endif // VCCC_ITERATOR_ITERATOR_TRAITS_LEGACY_FORWARD_ITERATOR_HPP_
